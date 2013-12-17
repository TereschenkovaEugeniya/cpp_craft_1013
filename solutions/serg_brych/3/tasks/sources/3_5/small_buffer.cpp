#define BOOST_FILESYSTEM_VERSION 3
#include <iostream> 
#include <fstream> 
#include <algorithm>
#include <stdint.h>
#include <map>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>


static const uint32_t MaxBufferSize = 2048;
static const uint32_t MaxPackageType = 100000;

struct Package
{
	uint32_t type;
	uint32_t time;
	uint32_t len;
};

struct Buffer
{
	uint32_t free_size;
	uint32_t count_of_stored_msg;
	Buffer():free_size(MaxBufferSize),count_of_stored_msg(0){}
};


// type std::map<type, count_of_saved_messages>
typedef std::map<uint32_t, uint32_t >  CountOfSavedMessage;
typedef std::map<uint32_t, Buffer >  CountOfSavedMessageTimed;

typedef std::map<uint32_t, double >  ResultContainer;

typedef boost::shared_ptr<std::ifstream> shared_ifstream;
typedef boost::shared_ptr<std::ofstream> shared_ofstream;

boost::mutex m;

ResultContainer Result;


void ThreadFunction(shared_ifstream input_file)
{
	CountOfSavedMessage all_saved_message;
	CountOfSavedMessageTimed msg_of_time;
	uint32_t all_time = 0;
	bool first_msg = true;
	Package current_package = {0};
	uint32_t current_time = 0;

	if(input_file->is_open())
	{
		while(input_file->read(reinterpret_cast<char*>(&current_package), sizeof(Package)))
		{
			if(first_msg)
			{
				first_msg = false;
				current_time = current_package.time;
				all_time++;
			}
			if(current_package.type <= MaxPackageType)
			{
				if(current_time != current_package.time)
				{
					current_time = current_package.time;

					std::for_each(msg_of_time.begin(), msg_of_time.end(), [&](CountOfSavedMessageTimed::value_type & i)
					{
						all_saved_message[i.first] += (i.second.count_of_stored_msg);
					});

					msg_of_time.clear();
					all_time++;
				}
				
				if(msg_of_time[current_package.type].free_size > sizeof(Package) + current_package.len)
				{
					msg_of_time[current_package.type].free_size -= sizeof(Package) + current_package.len;
					msg_of_time[current_package.type].count_of_stored_msg += 1;
				}
			}
			if(current_package.len > 0)
			{
				std::streampos pos = input_file->tellg();
				input_file->seekg(pos + static_cast<std::streamoff>(current_package.len));	
			}
			
		}

		std::for_each(msg_of_time.begin(), msg_of_time.end(), [&](CountOfSavedMessageTimed::value_type & i)
		{
			all_saved_message[i.first] += (i.second.count_of_stored_msg);
		});
		
		{
			boost::lock_guard<boost::mutex> l(m);
			std::for_each(all_saved_message.begin(), all_saved_message.end(), [&] (CountOfSavedMessage::value_type & i)
			{
				Result[i.first] +=  (static_cast<double>(i.second) / static_cast<double>(all_time));
			});
		}
	}
}


int main() 
{ 
	const std::string path = BINARY_DIR"/";
	const boost::regex filter("input_(\\d{3}).txt$");
	boost::filesystem::directory_iterator end_it;
	boost::thread_group th_group;
	
	for(boost::filesystem::directory_iterator it(path); it != end_it; it++)
	{
		if(boost::filesystem::is_regular_file(it->status()))
		{
			boost::smatch what;
			std::string temp_string = it->path().leaf().string();
			if( boost::regex_match(temp_string, what, filter ) )
			{
				boost::thread *th = new boost::thread(ThreadFunction, boost::make_shared<std::ifstream>(it->path().c_str(), std::ifstream::binary));
				th_group.add_thread(th);
			}
		}
	}

	th_group.join_all();

	//Put result to output_file
	std::ofstream output_file(BINARY_DIR"/output.txt", std::ifstream::binary);

	std::for_each(Result.begin(), Result.end(), [&output_file] (ResultContainer::value_type & i)
	{
		output_file.write(reinterpret_cast<const char*>(&i.first), sizeof(i.first));
		output_file.write(reinterpret_cast<const char*>(&i.second), sizeof(i.second));
	});


	return 0;
}
