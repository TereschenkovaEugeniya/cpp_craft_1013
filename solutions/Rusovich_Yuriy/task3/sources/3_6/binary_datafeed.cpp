#include <reader.h>
#include <map>

#pragma pack(push,1)

struct OutDataFeed
{
	char stockName[9];
    unsigned __int32 date;
    double price;
    unsigned __int32 volume;
    double f2;
};

#pragma pack(pop)

std::map < std::string, std::ofstream *> outputFiles;

int main()
{
	binary_reader::Reader reader(std::string(SOURCE_DIR) + "/input.txt");

	if (reader.isOpenedFile())
	{
		DataFeed dataFeed;
		while (reader.readDataFeed(&dataFeed))
		{
			std::string stockName;
			stockName.assign(dataFeed.stockName, 8);
			if (outputFiles.find(stockName) == outputFiles.end())
			{
				std::string fileName = std::string(SOURCE_DIR) + "/output_" + stockName + ".txt";

				std::ofstream *outputFile = new std::ofstream(fileName.c_str()
					, std::ios_base::binary|std::ios_base::out|std::ios_base::trunc );
				outputFiles.insert(std::pair< std::string, std::ofstream *>(stockName, outputFile));
			}
			
			if (outputFiles[stockName]->is_open())
			{
				OutDataFeed outDataFeed;
				memcpy(&outDataFeed.stockName, &dataFeed.stockName, 8);

				char year[4];
				char month[2];
				char day[2];

				memcpy(year, &(dataFeed.year), 4);
				memcpy(month, &(dataFeed.month), 2);
				memcpy(day, &(dataFeed.day), 2);

				outDataFeed.date = (atoi(year) - 1) * 372
					+ (atoi(month) - 1) * 31
					+ atoi(day);

				outDataFeed.price = dataFeed.vwap;
				outDataFeed.volume = dataFeed.volume;
				outDataFeed.f2 = dataFeed.f2;
				
				outputFiles[stockName]->write(reinterpret_cast<char*>(&outDataFeed), sizeof(outDataFeed));
			}
		}
		for(std::map < std::string, std::ofstream *>::iterator it = outputFiles.begin();
			it != outputFiles.end(); )
		{
			it->second->close();
			delete it->second;
			it = outputFiles.erase(it);
		}
	}
}
