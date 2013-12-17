#include "reader.h"


namespace binary_reader
{
	Reader::Reader(const std::string &nameFile)
		: inputFile(nameFile.c_str(), std::ios_base::binary|std::ios_base::in)
	{
	}

	Reader::~Reader()
	{
		inputFile.close();
	}

	bool Reader::isOpenedFile()
	{
		return inputFile.is_open();
	}

	bool Reader::readMessageData(MessageData *messageData)
	{
		if (!inputFile.read(reinterpret_cast<char*>(&messageData->type), sizeof(unsigned __int32)))
			return false;
		if (!inputFile.read(reinterpret_cast<char*>(&messageData->time), sizeof(unsigned __int32)))
			return false;
		if (!inputFile.read(reinterpret_cast<char*>(&messageData->len), sizeof(unsigned __int32)))
			return false;
		char *msg = new char[messageData->len];
		if (!inputFile.read(reinterpret_cast<char*>(msg), messageData->len * sizeof(char)))
		{		
			delete [] msg;
			return false;
		}
		messageData->message.assign(msg, messageData->len);

		delete [] msg;
		return true;
	}

	bool Reader::readDataFeed(DataFeed *dataFeed)
	{
		if(!inputFile.read(reinterpret_cast<char*>(dataFeed), sizeof(DataFeed)))
		{
			return false;
		}
		return true;
	}
}
