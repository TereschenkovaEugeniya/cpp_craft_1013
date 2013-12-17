#include <reader.h>

#include <iostream>
#include <sstream>

#include <boost/thread.hpp>

const static int needTypes = 4;

void startWorkDataThread(const std::string &input)
{
	binary_reader::Reader reader(std::string(SOURCE_DIR) + "/input_" + input + ".txt");
			
	unsigned __int32 currentTime = 0;

	if (reader.isOpenedFile())
	{
		std::ofstream outputFile((std::string(SOURCE_DIR) + "/output_" + input + ".txt").c_str()
		, std::ios_base::binary|std::ios_base::out|std::ios_base::trunc);

		MessageData tradesData;

		while (reader.readMessageData(&tradesData))
		{
			if (tradesData.type > needTypes)
				continue;

			if (tradesData.time + 2 > currentTime || currentTime < tradesData.time)
			{
				if (outputFile)
				{
					outputFile.write(reinterpret_cast<char*>(&tradesData.type), sizeof(unsigned __int32));
					outputFile.write(reinterpret_cast<char*>(&tradesData.time), sizeof(unsigned __int32));
					outputFile.write(reinterpret_cast<char*>(&tradesData.len), sizeof(unsigned __int32));
					outputFile.write(tradesData.message.c_str(), tradesData.len * sizeof(char));
				}
			}
			if (currentTime < tradesData.time)
				currentTime = tradesData.time;
		}
		outputFile.close();
	}
}

int main()
{
	static const size_t threadsCount = 1000;
	boost::thread_group groupTradesData;
	for( size_t i = 1; i < threadsCount; ++i )
	{
		std::stringstream ss;
		ss << i;
		std::string number = ss.str();

		groupTradesData.create_thread( boost::bind( startWorkDataThread, std::string("000" + number).substr(number.size(), 3) ) );
	}
	groupTradesData.join_all();
}
