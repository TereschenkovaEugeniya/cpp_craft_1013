#include <stdint.h>
#include <fstream>
#include <string>

struct Data
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

uint32_t days_count(const char* date)
{
	int month;
    int day;
    int year;
	std::sscanf(date, "%4d%2d%2d", &year, &month, &day);
	return ((year - 1) * 372 +  (month  - 1)* 31 + day);
}

void get_data(std::ifstream &file, Data &data)
{
	file.read(data.stock_name, sizeof(data.stock_name));
	file.read(data.date_time, sizeof(data.date_time));
	file.read(reinterpret_cast<char*>(&data.price), sizeof(data.price));
	file.read(reinterpret_cast<char*>(&data.vwap), sizeof(data.vwap));
	file.read(reinterpret_cast<char*>(&data.volume), sizeof(data.volume));
	file.read(reinterpret_cast<char*>(&data.f1), sizeof(data.f1));
	file.read(reinterpret_cast<char*>(&data.t1), sizeof(data.t1));
	file.read(reinterpret_cast<char*>(&data.f2), sizeof(data.f2));
	file.read(reinterpret_cast<char*>(&data.f3), sizeof(data.f3));
	file.read(reinterpret_cast<char*>(&data.f4), sizeof(data.f4));
}

void set_data(std::ofstream &file, Data &data)
{
	file.write(data.stock_name, 9);
	uint32_t days = days_count(data.date_time);
	file.write(reinterpret_cast<char*>(&days), sizeof(days));
	data.price = data.vwap;
	file.write(reinterpret_cast<char*>(&data.price), sizeof(data.price));
	file.write(reinterpret_cast<char*>(&data.volume), sizeof(data.volume));
	file.write(reinterpret_cast<char*>(&data.f2), sizeof(data.f2));
}

int main()
{
	std::ifstream input_file( BINARY_DIR "/input.txt", std::ios::binary|std::ios::ate );
	if ( !input_file.is_open() )
		return -1;
	std::ofstream output_file( BINARY_DIR "/output.txt", std::ios::binary );
	if ( !output_file.is_open() )
		return -1;
	size_t file_size;
	file_size = static_cast<size_t>(input_file.tellg());
	input_file.seekg (0, std::ios::beg);
	size_t read_size = 0;
	struct Data data;

	while(read_size < file_size)
	{
		get_data(input_file, data);
		read_size = static_cast<size_t>(input_file.tellg());
		set_data(output_file, data);
	}
	input_file.close();
	output_file.close();
	return 1;
}
