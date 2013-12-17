#include "reader.h"
#include <fstream>


int binary_reader::read_binary(std::fstream &fid, stock_ex_dat &data)
{

	if(fid.read((char*)&(data.TYPE), 3*sizeof(uint32_t)))
	{
		char* buffer = new char [data.LEN];
		if (fid.read(buffer, data.LEN))
		{
			data.MSG.assign(buffer, data.LEN);
		}
		else 
		{ 
			delete [] buffer;
			return -1;
		}


		delete [] buffer;
	}
	else 
		return -1;

	return 0;
}

