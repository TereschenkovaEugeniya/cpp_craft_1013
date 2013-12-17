#include "reader.h"

void binary_reader::read_struct(std::ifstream& file, Data &data, uint32_t type_max)
{
	file.read((char*) &data.type, sizeof(data.type));
	if(data.type > type_max)
		exit(1);
	file.read((char*) &data.time, sizeof(data.time));
	file.read((char*) &data.len, sizeof(data.len));
	if(data.len > 0)
	{
		data.msg = new char [data.len];
		file.read((char*) data.msg, data.len);
	}
}