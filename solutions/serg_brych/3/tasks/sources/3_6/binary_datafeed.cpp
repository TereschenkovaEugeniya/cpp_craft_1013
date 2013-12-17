#include <iostream> 
#include <fstream> 
#include <algorithm>
#include <stdint.h>
#include <map>
#include <sstream>

typedef std::map<std::string, std::ofstream > OutPutStreams;


struct Package
{
	char stock_name[8];
	char data_time[8];
	double price;
	double vwap;
	uint32_t volume;
	double f1;
	double t1;
	double f2;
	double f3;
	double f4;
};

std::istream& operator>> (std::istream& s, Package &p)
{
	s.read(p.stock_name, sizeof(p.stock_name));
	s.read(p.data_time, sizeof(p.data_time));
	s.read(reinterpret_cast<char*>(&p.price), sizeof(p.price));
	s.read(reinterpret_cast<char*>(&p.vwap), sizeof(p.vwap));
	s.read(reinterpret_cast<char*>(&p.volume), sizeof(p.volume));
	s.read(reinterpret_cast<char*>(&p.f1), sizeof(p.f1));
	s.read(reinterpret_cast<char*>(&p.t1), sizeof(p.t1));
	s.read(reinterpret_cast<char*>(&p.f2), sizeof(p.f2));
	s.read(reinterpret_cast<char*>(&p.f3), sizeof(p.f3));
	s.read(reinterpret_cast<char*>(&p.f4), sizeof(p.f4));
	return s;
}


std::ostream& operator<< (std::ostream& s, const Package &p)
{
	uint32_t date = 0;
	int month;
    int day;
    int year;

	std::sscanf(p.data_time, "%4d%2d%2d", &year, &month, &day);
	date = (year - 1) * 372 +  (month  - 1)* 31 + day;
	
	s.write(p.stock_name, sizeof(p.stock_name));
	s << '\0';
	s.write(reinterpret_cast<const char*>(&date), sizeof(date));
	s.write(reinterpret_cast<const char*>(&p.vwap), sizeof(p.vwap));
	s.write(reinterpret_cast<const char*>(&p.volume), sizeof(p.volume));
	s.write(reinterpret_cast<const char*>(&p.f2), sizeof(p.f2));

	return s;
}

int main() 
{ 
	OutPutStreams output_streams;
	Package current_package ={0};
	
	std::ifstream input_file(BINARY_DIR"/input.txt", std::ifstream::binary);
	

	if(input_file.is_open())
	{
		while(input_file >> current_package)
		{
			std::string stock_name(current_package.stock_name, 8);
			if(!output_streams[stock_name].is_open())
			{
				std::stringstream ss;

				ss << BINARY_DIR"/output_" << stock_name << ".txt";
				output_streams[stock_name].open(ss.str(), std::ofstream::binary);
			}
			output_streams[stock_name] << current_package;
		}
	}
	
	return 0;
}