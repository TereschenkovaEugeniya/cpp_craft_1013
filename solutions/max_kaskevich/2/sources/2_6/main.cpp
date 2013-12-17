#include <stdint.h>
#include <fstream>
#include "message.h"


struct message2_6
{
    char stock_name[8];
    char date_time[8];

    double price;        
    double vwap;

    uint32_t volume;

    double f1; 
    double t1;
    double f2; 
    double f3;
    double f4;
}; 

std::istream& operator >> (std::istream& input, message2_6& m)
{
    read(input, m.stock_name, sizeof(m.stock_name));
    read(input, m.date_time, sizeof(m.date_time));
    read(input, &m.price);
    read(input, &m.vwap);
    read(input, &m.volume);
    read(input, &m.f1);
    read(input, &m.t1);
    read(input, &m.f2);
    read(input, &m.f3);
    read(input, &m.f4);
    return input;
}

std::ostream& operator << (std::ostream& output, message2_6& m)
{
    write(output, m.stock_name, sizeof(m.stock_name));
    output << '\0';

    uint32_t year, month, day;
    std::sscanf(m.date_time, "%4d%2d%2d", &year, &month, &day);
    uint32_t all_days = (year - 1) * 372 + (month - 1) * 31 + day;
    write(output, &all_days);

    write(output, &m.vwap);
    write(output, &m.volume);
    write(output, &m.f2);
    return output;
}

int main( int argc, char* argv[] )
{
    std::ifstream input(BINARY_DIR "/input.txt", std::ios::binary);
    std::ofstream output(BINARY_DIR "/output.txt", std::ios::binary);
    if(!input || !output)
    {
        return 1;
    }
    message2_6 m;
    while (input >> m && output << m)
    {}

}