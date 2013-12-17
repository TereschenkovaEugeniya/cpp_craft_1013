#include <reader.h>

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

int main()
{
	binary_reader::Reader reader(std::string(SOURCE_DIR) + "/input.txt");
	std::ofstream outputFile(SOURCE_DIR "/output.txt", std::ios_base::binary|std::ios_base::out|std::ios_base::trunc );
	
	if (reader.isOpenedFile())
	{
		DataFeed dataFeed;

		while (reader.readDataFeed(&dataFeed))
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
			
			outputFile.write(reinterpret_cast<char*>(&outDataFeed), sizeof(outDataFeed));
		}
	}
	outputFile.close();
}

