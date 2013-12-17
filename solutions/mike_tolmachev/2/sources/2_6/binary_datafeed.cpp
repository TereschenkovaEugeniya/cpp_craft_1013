#include <fstream>
#include <string>
#include <stdint.h>


struct Data
{
    char stock_name[8];//8?
    char date_time[8];
    double price;
    double vwap;
    uint32_t volume;
    double datas[5];
};

std::istream& operator >> (std::istream& in, Data& data)
{
	in.read(data.stock_name, 8);//8?
	in.read(data.date_time, 8);
	in.read(reinterpret_cast<char*>(&data.price), sizeof(double));
	in.read(reinterpret_cast<char*>(&data.vwap), sizeof(double));
	in.read(reinterpret_cast<char*>(&data.volume), sizeof(uint32_t));
	in.read(reinterpret_cast<char*>(data.datas), sizeof(data.datas));
    return in;
}

std::ostream& operator << (std::ostream& out, Data& data)
{
    out.write(data.stock_name, 8);//stock_name 8?
    out.write("\0", 1);// 8?
    //write date
	uint32_t day = 0;
	uint32_t month = 0;
	uint32_t year = 0;
	std::sscanf(data.date_time, "%4u%2u%2u", &year, &month, &day);
	uint32_t date = 372*(year - 1) + 31*(month-1) + day;
	out.write(reinterpret_cast<char*>(&date), sizeof(uint32_t));
	//
    out.write(reinterpret_cast<char*>(&data.vwap), sizeof(double));//price=vwap
    out.write(reinterpret_cast<char*>(&data.volume), sizeof(uint32_t));//volume
	out.write(reinterpret_cast<char*>(&data.datas[2]), sizeof(double));//f2

	return out;
}

int main()
{
    std::ifstream in(BINARY_DIR "/input.txt", std::ifstream::binary);
    std::ofstream out(BINARY_DIR "/output.txt", std::ofstream::binary);
    
	if (in.is_open())
	{
		Data data;

		while(in >> data)
		{
			out << data;
		}
	}

    in.close();
    out.close();
}