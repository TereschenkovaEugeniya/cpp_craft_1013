#ifndef _BINARY_READER_READER_H_
#define _BINARY_READER_READER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>


namespace binary_reader
{

	#pragma pack(push,1)

	typedef struct stock_ex_dat
	{
		uint32_t TYPE;
		uint32_t TIME; 
		uint32_t LEN;
		std::string MSG;
	} stock_ex_dat;

	#pragma pack(pop)

	int read_binary(std::fstream &fid, stock_ex_dat &data);
}

#endif // _BINARY_READER_READER_H_
