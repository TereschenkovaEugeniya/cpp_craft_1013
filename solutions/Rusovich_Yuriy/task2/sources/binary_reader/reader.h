#ifndef _BINARY_READER_READER_H_
#define _BINARY_READER_READER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

struct MessageData
{
	unsigned __int32 type;
	unsigned __int32 time; 
	unsigned __int32 len;
	std::string message;
 };

#pragma pack(push,1)

struct DataFeed
{
	char stockName[8];
	char year[4];
	char month[2];
	char day[2];
	double price;
	double vwap;
	unsigned __int32 volume;
	double f1;
	double t1;
	double f2;
	double f3;
	double f4;
};

#pragma pack(pop)

namespace binary_reader
{
	class Reader
	{
	public:
		Reader(const std::string &nameFile);
		virtual ~Reader();

		bool isOpenedFile();
		bool readMessageData(MessageData *messageData);
		bool readDataFeed(DataFeed *dataFeed);
	
	private:
		std::ifstream inputFile;
	};
}

#endif // _BINARY_READER_READER_H_
