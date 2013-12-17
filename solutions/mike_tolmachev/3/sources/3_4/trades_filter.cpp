#include "reader.h"
#include <memory>
#include <map>
#include <boost/thread/thread.hpp>
#include <boost/format.hpp>

using namespace binary_reader;

static const size_t buf_size = 4096;

void ThreadFun(size_t& file_numb)
{
	std::string numb;
	numb = boost::str(boost::format("%03u") % file_numb);

	std::ifstream in(BINARY_DIR "/input_"+numb+".txt", std::ifstream::binary);
    std::ofstream out(BINARY_DIR "/output_"+numb+".txt", std::ofstream::binary);

    uint32_t T = 0;
    
    if (in.is_open())
    {
        Msg msg;

        while(in >> msg)
        {
            if (((msg.time + 2) > T) && (1 <= msg.type) && (msg.type <= 4))
            {
                out << msg;

				char buf[4096];
				uint32_t len = 0;
                for (len = msg.len; len > buf_size; len -= buf_size)
                {
					in.read(buf, buf_size);
					out.write(buf, buf_size);
                }
				in.read(buf, len);
				out.write(buf, len);

                if (msg.time > T)
                {
                    T = msg.time;
                }
            }
            else
            {
                in.seekg(msg.len, std::ios::cur);
            }
        }
    }

    in.close();
    out.close();
}

int main()
{
	boost::thread_group thread_group;
	for(size_t i = 1; i <= 999; ++i)
	{
		thread_group.create_thread(boost::bind(ThreadFun, i));
	}

	thread_group.join_all();
}