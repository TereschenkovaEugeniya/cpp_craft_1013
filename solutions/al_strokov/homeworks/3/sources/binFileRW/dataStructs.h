#ifndef DATASTRUCTS_INCLUDE
#define DATASTRUCTS_INCLUDE

#include <string>
#include <stdint.h>
#include <stdlib.h>

struct dataStruct1_t{
	uint32_t type;
	uint32_t time;
	uint32_t len;
	std::string msg;
};

struct dataStruct2_t{
	std::string stockName;
	std::string date;
	double price;
	double vwap;
	uint32_t volume;
	double f1;
	double t1;
	double f2;
	double f3;
	double f4;

	static const size_t stockNameLength = 8;
	static const size_t stockNameOutputLength = 9;
	static const size_t dateLength = 8;
};

#endif
