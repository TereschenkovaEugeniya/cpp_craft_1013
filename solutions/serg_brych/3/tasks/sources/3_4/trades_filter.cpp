#define BOOST_FILESYSTEM_VERSION 3
#include <iostream> 
#include <fstream> 
#include <algorithm>
#include <stdint.h>
#include <queue>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

static const uint32_t max_type = 4u;
static const uint32_t BufferSize = 2048;

struct deal
{
	uint32_t type;
	uint32_t time;
	uint32_t len;
};


typedef boost::shared_ptr<std::ifstream> shared_ifstream;
typedef boost::shared_ptr<std::ofstream> shared_ofstream;
typedef std::queue<std::pair<shared_ifstream, shared_ofstream> > TaskQueue;

boost::mutex mutex_task;

TaskQueue task_queue;


void ThreadFunction()
{
	uint32_t max_time = 0;
	char buffer[BufferSize] = {0};
	deal current_deal ={0};
	bool store_deal = false;
	shared_ifstream input_file;
	shared_ofstream output_file;

	mutex_task.lock();
	while(!task_queue.empty())
	{
		input_file = task_queue.front().first;
		output_file = task_queue.front().second;
		task_queue.pop();
		mutex_task.unlock();
		

		if(input_file->is_open() && output_file->is_open())
		{
			while(input_file->read(reinterpret_cast<char*>(&current_deal), sizeof(deal)))
			{
				store_deal = false;

				if((current_deal.type > 0) && ( current_deal.type <= max_type))
				{
					if(max_time < current_deal.time)
					{
						max_time = current_deal.time;
						store_deal = true;
					}
					else
					{
						if((current_deal.time + 2) > max_time)
						{
							store_deal = true;
						}
					}
				}

				if(store_deal)
				{
					output_file->write(reinterpret_cast<char*>(&current_deal), sizeof(deal));
					uint32_t read_bytes_msg = 0;
					uint32_t bytes_to_read = 0;
					while(read_bytes_msg < current_deal.len)
					{
						bytes_to_read = BufferSize < current_deal.len? BufferSize: current_deal.len;

						input_file->read(buffer, bytes_to_read);
						output_file->write(buffer, bytes_to_read);
						read_bytes_msg += bytes_to_read;
					}
				}
				else
				{
					std::streampos pos = input_file->tellg();
					input_file->seekg(pos + static_cast<std::streamoff>(current_deal.len));
				}
			}
		}
		mutex_task.lock();
	}
	mutex_task.unlock();
}

int main() 
{ 
	const std::string path = BINARY_DIR"/";
	const boost::regex filter("input_(\\d{3}).txt$");
	boost::filesystem::directory_iterator end_it;
	boost::thread_group th_group;
	uint32_t count_of_threads = boost::thread::hardware_concurrency() > 2 ? boost::thread::hardware_concurrency() : 1;

	for(boost::filesystem::directory_iterator it(path); it != end_it; it++)
	{
		if(boost::filesystem::is_regular_file(it->status()))
		{
			boost::smatch what;
			std::string temp_string = it->path().leaf().string();
			if( boost::regex_match(temp_string, what, filter ) )
			{
				std::stringstream output_path;
				output_path << it->path().parent_path().string() << "/output_" << what[1] << ".txt\0"; 
				{
					boost::lock_guard<boost::mutex> lock_task(mutex_task);
					task_queue.push(std::make_pair<shared_ifstream, shared_ofstream>(boost::make_shared<std::ifstream>(it->path().c_str(), std::ifstream::binary),
																		boost::make_shared<std::ofstream>(output_path.str(), std::ofstream::binary)));
				}
				
			}
		}
	}
	
	for(uint32_t i = 0; i < count_of_threads; i++)
	{
		th_group.create_thread(ThreadFunction);
	}
	
	
	
	th_group.join_all();
		
	return 0;
}