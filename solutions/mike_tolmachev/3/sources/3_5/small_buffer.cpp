#include "../binary_reader/reader.h"
#include <memory>
#include <map>
#include <string>
#include <boost/thread/thread.hpp>
#include <boost/format.hpp>
#include <boost/thread/mutex.hpp>

using namespace binary_reader;

static const uint32_t MSG_SIZE = 2048;
static const uint32_t DATA_SIZE = 3*sizeof(uint32_t);
std::map<uint32_t, double> average;//type, average
boost::mutex mtx;

void ThreadFun(size_t& file_numb)
{
	std::string numb;
	numb = boost::str(boost::format("%03u") % file_numb);

	std::ifstream in(BINARY_DIR "/input_"+numb+".txt", std::ifstream::binary);
    
    if (in.is_open())
    {
		Msg msg;
		    
        std::map<uint32_t, std::map<uint32_t, std::pair<uint32_t, double>>> msgs;//<TYPE,<TIME<LEN, count>>>

		while(in >> msg)
        {
			in.seekg(msg.len, std::ios::cur);

			uint32_t msg_size = DATA_SIZE + msg.len;

            auto it = msgs[msg.type].find(msg.time);
            if (it == msgs[msg.type].end())
            {
                auto new_elem = msgs[msg.type].insert(std::make_pair(msg.time, std::make_pair(0, 0)));
                it = new_elem.first;
            }

			if ((it->second.first + msg_size) <= MSG_SIZE)
            {
				it->second.first += msg_size;
				++it->second.second;
            }
        }

		boost::mutex::scoped_lock lock(mtx);
		for (auto it = msgs.begin(); it != msgs.end(); ++it)
		{
			uint32_t type = it->first;
			auto iter = average.find(type);
			if (iter == average.end())
            {
				iter = average.insert(average.begin(), std::make_pair(type, 0));
            }
			
			double count = 0;
			for (auto it_t = it->second.begin(); it_t != it->second.end(); ++it_t)
			{
				count += it_t->second.second;
			}
			count /= it->second.size();
			iter->second += count;
		}
    }

    in.close();
}

int main()
{
	boost::thread_group thread_group;
	for(size_t i = 1; i <= 999; ++i)
	{
		thread_group.create_thread(boost::bind(ThreadFun, i));
	}

	thread_group.join_all();

	std::ofstream out(BINARY_DIR "/output.txt", std::ofstream::binary);
	for (auto it = average.begin(); it != average.end(); ++it)
    {
		uint32_t type = it->first;
		double count = it->second;
        out.write(reinterpret_cast<char*>(&type), sizeof(uint32_t));
		out.write(reinterpret_cast<char*>(&count), sizeof(double));
    }

	out.close();
}