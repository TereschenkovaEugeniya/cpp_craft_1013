// puzzle_3.cpp : Defines the entry point for the console application.
//
#include <fstream>
#include <stdint.h>
#include <vector>
#include <boost/thread.hpp>
#include <boost/format.hpp>
#include "boost/filesystem.hpp" 
#include <boost/noncopyable.hpp>

#include "base.hpp"

namespace task34
{
    using namespace task3;

    enum MessageType : uint32_t
    {
        MarketOpen = 1,
        Trade = 2,
        Quote = 3,
        MarketClose = 4
    };

    struct Data : boost::noncopyable
    {
        uint32_t type;
        uint32_t time;
        uint32_t length;        

        explicit Data(){}

        friend std::istream& operator >> (std::istream& in, Data& m);
        friend std::ostream& operator << (std::ostream& ou, Data& m);
    };

    std::istream& operator >> (std::istream& in, Data& m)
    {
        ReadValue(in, m.type);
        ReadValue(in, m.time);
        ReadValue(in, m.length);        
        return in;
    }

    std::ostream& operator << (std::ostream& out, Data& m)
    {
        WriteValue(out, m.type);
        WriteValue(out, m.time);
        WriteValue(out, m.length);    
        return out;
    }
    
    void CopyNData(std::istream& in, std::ostream& out, uint32_t n_size)
    {
        std::vector<char> buffer(c_fs_cluster_size);
        uint32_t read_size = c_fs_cluster_size;
        while (read_size < n_size)
        {
            in.read(&buffer[0], c_fs_cluster_size);
            out.write(&buffer[0], c_fs_cluster_size);
            read_size += c_fs_cluster_size;
        }

        in.read(&buffer[0], n_size%c_fs_cluster_size);
        out.write(&buffer[0], n_size%c_fs_cluster_size);

    }

    void SortData(std::istream& in, std::ostream& out, uint32_t current_time)
    {
        Data d;        
        while (in >> d)
        {
            if (((d.time+2) > current_time) 
                && ((d.type >= MarketOpen)
                    && (d.type <= MarketClose)))
            {
                out << d;
                CopyNData(in, out, d.length);
            }
            else
            {
                in.ignore(d.length);
            }
            if (current_time < d.time)
                current_time = d.time;
        }
    };


    void ThreadTask34(const std::string in_files_name, const std::string out_files_name)
    {
        std::ifstream in (in_files_name, std::ios::binary);
        std::ofstream out (out_files_name, std::ios::binary);
        if (!in.is_open() || !out.is_open())
        {
            return;
        }
        task34::SortData(in, out, 0);
    }

 
} // task34
    

int main()
{    

    boost::thread_group threads;

	
    const std::string first_in_part(BINARY_DIR "/input_");
    const std::string first_out_part(BINARY_DIR "/ouutput_");
    const std::string txt_part(".txt");


    for(uint32_t i = 1; i< 1000; ++ i)
    {
        std::string num_part = boost::str(boost::format("%03u") % i);
        std::string in_file = first_in_part + num_part + txt_part;
        if (boost::filesystem::exists (in_file))
        {
            std::string out_file = first_out_part + num_part + txt_part;
            threads.create_thread(boost::bind(task34::ThreadTask34, in_file, out_file));
        }

    }

    threads.join_all();    
	return 0;
}

