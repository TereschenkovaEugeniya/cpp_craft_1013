// puzzle_3.cpp : Defines the entry point for the console application.
//
#include <fstream>
#include <stdint.h>
#include <algorithm>
#include <boost/thread.hpp>
#include <boost/format.hpp>
#include "boost/filesystem.hpp" 
#include <boost/noncopyable.hpp>
#include <boost/noncopyable.hpp>

#include <vector>

#include "base.hpp"

namespace task35
{
    using namespace task3;

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

    struct FinalStatistic
    {
        double message_sum;
        uint32_t count;
        FinalStatistic():message_sum(0), count(0){}
    };

    typedef std::vector<DataStatistic> TypeStatistic;
    typedef std::vector<FinalStatistic> Store;

    void AnalyzeData(std::istream& in, Store& store, boost::mutex& store_guard)
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

        boost::mutex::scoped_lock lock(store_guard);


        for(uint32_t i = 0; i <= c_max_message_type; ++i)
            if (stat[i].time_count)
            {
                
                store[i].message_sum += stat[i].message_count*1.0/stat[i].time_count;
                ++store[i].count;
            }
    };




    void ThreadTask35(const std::string in_files_name, Store& store, boost::mutex& store_guard)
    {
        std::ifstream in (in_files_name, std::ios::binary);
        if (!in.is_open())
        {
            return;
        }
        task35::AnalyzeData(in, store, store_guard);
    }
     
} // task35
    

int main()
{
    boost::mutex store_guard;

    task35::Store store(task35::c_max_message_type+1);
    boost::thread_group threads;

	
    const std::string first_in_part(BINARY_DIR "/input_");
    const std::string out_file (BINARY_DIR "/output.txt");
    const std::string txt_part(".txt");


    for(uint32_t i = 1; i< 1000; ++ i)
    {
        std::string num_part = boost::str(boost::format("%03u") % i);
        std::string in_file = first_in_part + num_part + txt_part;
        if (boost::filesystem::exists (in_file))
        {
 
            threads.create_thread(boost::bind(task35::ThreadTask35, in_file, boost::ref(store), boost::ref(store_guard)));
        }

    }

    threads.join_all();   

    std::ofstream out (out_file, std::ios::binary);
    if ( !out.is_open())
    {
        return -1;
    }


    for(uint32_t i = 0; i <= task35::c_max_message_type; ++i)
        if (store[i].count)
        {
            task3::WriteValue(out, i);
            task3::WriteValue(out, store[i].message_sum/store[i].count);
        }

 
	return 0;
}

