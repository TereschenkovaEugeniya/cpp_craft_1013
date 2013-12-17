// puzzle_2.cpp : Defines the entry point for the console application.
//
#include <fstream>
#include <stdint.h>
#include <algorithm>
#include <boost\noncopyable.hpp>

#include <vector>

#include "base.hpp"

namespace task25
{
    using namespace task2;

    static const uint32_t c_max_message_type = 100000;
    static const uint32_t c_max_message_size = 2048;

    struct Data : boost::noncopyable
    {
        uint32_t type;
        uint32_t time;
        uint32_t length; 
        uint64_t size;
        explicit Data(){}            
        friend std::istream& operator >> (std::istream& in, Data& m);
    };

    std::istream& operator >> (std::istream& in, Data& m)
    {
        ReadValue(in, m.type);
        ReadValue(in, m.time);
        ReadValue(in, m.length);        
        m.size = sizeof(m.type) + sizeof(m.time) + sizeof(m.length) + m.length;
        in.ignore(m.length);
        return in;
    }

    struct DataStatistic
    {
        uint64_t message_count;
        uint64_t time_count;
        uint64_t current_buff_size;
        uint32_t current_time;        
        explicit DataStatistic(): message_count(0), time_count(0), current_time(0), current_buff_size(0) {}
    };   

    typedef std::vector<DataStatistic> TypeStatistic;

    void AnalyzeData(std::istream& in, std::ostream& out, uint32_t current_time)
    {
        TypeStatistic stat(c_max_message_type+1);
        Data message;
        while (in >> message)
        {
            DataStatistic& act = stat[message.type];

            if ((act.message_count) &&(act.current_time != message.time))
                act.current_buff_size = 0;

            if (act.current_buff_size + message.size <= c_max_message_size)
            {
                act.current_time = message.time;
                if (act.current_buff_size == 0) // first message in this time
                {
                    ++act.time_count;
                }

                act.current_buff_size += message.size;
                ++act.message_count;
            }
        }

        for(uint32_t i = 0; i <= c_max_message_type; ++i)
            if (stat[i].time_count)
            {
                WriteValue(out, i);
                WriteValue(out, stat[i].message_count*1.0/stat[i].time_count);
            }
    };
     
} // task25
    

int main()
{
    std::ifstream in (BINARY_DIR "/input.txt", std::ios::binary);
    std::ofstream out (BINARY_DIR "/output.txt", std::ios::binary);
    if (!in.is_open() || !out.is_open())
    {
        return 1;
    }
    task25::AnalyzeData(in, out, 0);
	return 0;
}

