// puzzle_3.cpp : Defines the entry point for the console application.
//
#include <fstream>
#include <stdint.h>
#include <map>

#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>


#include <boost/thread.hpp>
#include <boost/format.hpp>
#include "boost/filesystem.hpp" 
#include <boost/noncopyable.hpp>
#include "base.hpp" 

namespace task36
{
    using namespace task3;

    struct DataIn : boost::noncopyable
    {        
        fix_array<char, 8>::type stock_name;
        fix_array<char, 8>::type date_time;

        double price;        
        double vwap;
        uint32_t volume;

        double f1; 
        double t1;
        double f2; 
        double f3;
        double f4;

        explicit DataIn(){}
    private:
        friend std::istream& operator >> (std::istream& in, DataIn& m);
    };

    std::istream& operator >> (std::istream& in, DataIn& m)
    {
        ReadValue(in, m.stock_name);
        ReadValue(in, m.date_time);
        ReadValue(in, m.price);
        ReadValue(in, m.vwap);
        ReadValue(in, m.volume);
        ReadValue(in, m.f1);
        ReadValue(in, m.t1);
        ReadValue(in, m.f2);
        ReadValue(in, m.f3);
        ReadValue(in, m.f4);
        return in;
    }
    
    struct DataOutStream : boost::noncopyable
    {
        const DataIn& in;        
        explicit DataOutStream(const DataIn& in_obj):in(in_obj){}
    private:
        friend std::ostream& operator << (std::ostream& ou, DataOutStream& m);
    };

    static const uint32_t c_month_days = 31;
    static const uint32_t c_year_days = c_month_days* 12;

    std::ostream& operator << (std::ostream& out, DataOutStream& m)
    {
        WriteValue(out, m.in.stock_name);
        WriteValue(out, '\0');
        WriteValue(out, (boost::lexical_cast<uint32_t>(&m.in.date_time[0], 4)-1) * c_year_days
                        +(boost::lexical_cast<uint32_t>(&m.in.date_time[4], 2)-1) * c_month_days
                        +boost::lexical_cast<uint32_t>(&m.in.date_time[6], 2) );


        WriteValue(out, m.in.vwap);
        WriteValue(out, m.in.volume);
        WriteValue(out, m.in.f2);
        return out;
    }

    typedef std::map<std::string, std::ofstream> OutFiles;

    void DataFeed(std::istream& in, OutFiles& outs)
    {
        DataIn in_container;
        while (in >> in_container)
        {            
            std::string stock(in_container.stock_name, 8);
            std::ofstream& out = outs[stock];
            if (!out.is_open())
            {
                std::string out_name(BINARY_DIR "/output_");
                out_name.append(stock);
                out_name.append(".txt");
                out.open(out_name, std::ios::binary);
            }
            
            if (out.good())
            {
                out << DataOutStream(in_container);
            }
        }
    };

 
} // task36
    

int main()
{
    task36::OutFiles outs;


    std::ifstream in (BINARY_DIR "/input.txt", std::ios::binary);
    if (!in.is_open())
    {
        return 1;
    }

    task36::DataFeed(in, outs);
    return 0;
}

