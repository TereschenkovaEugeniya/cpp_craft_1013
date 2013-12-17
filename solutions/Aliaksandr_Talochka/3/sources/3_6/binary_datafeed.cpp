#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>



typedef boost::shared_ptr<std::fstream> ptr_fstream;


#pragma pack(push,1)

struct InDataStruct
{
	char Stock_name[8];
	
	char Year[4];
	char Month[2];
	char Day[2];

	double price;
	double vwap;
	uint32_t volume;

	double f1;
	double t1;
	double f2;
	double f3;
	double f4;
};


struct OutDataStruct
{
	char Stock_name[9];
	uint32_t date;
	double price;
	uint32_t volume;
	double f2;
};

#pragma pack(pop)


void write_struct(const OutDataStruct &OutData, std::map<std::string, ptr_fstream> &StreamMap)
{
	std::string StockName;
	std::map <std::string, ptr_fstream>::iterator it;
	StockName = OutData.Stock_name;

	it = StreamMap.find(StockName);
	
	if(it != StreamMap.end())
	{
		StreamMap[StockName]->write((char*)&OutData, sizeof(OutData));
	}
	else
	{
		std::string FileName = BINARY_DIR;
		FileName += "/Output_" + StockName + ".txt";
		ptr_fstream outFile (new std::fstream(FileName.c_str(), std::ios_base::binary|std::ios_base::out|std::ios_base::trunc));
		StreamMap.insert(std::pair<std::string, ptr_fstream>(StockName, outFile));

		StreamMap[StockName]->write((char*)&OutData, sizeof(OutData));
	}	
}

void close_files(std::map<std::string, ptr_fstream> &StreamMap)
{
	std::map <std::string, ptr_fstream>::iterator it;

	for (it = StreamMap.begin(); it != StreamMap.end(); ++it)
	{
		it->second->close();
	}
}




int main()
{
	std::fstream InFile;
	std::fstream OutFile;

	struct InDataStruct InDat;
	struct OutDataStruct OutDat;
	char Year[4];
	char Month[2];
	char Day[2];

	std::map<std::string, ptr_fstream>StreamMap; 


	InFile.open(BINARY_DIR"/Input.txt",std::ios_base::binary|std::ios_base::in);

	if(InFile)
	{
		while(InFile.read((char*)&InDat, sizeof(InDat)))
		{
			memcpy(&(OutDat.Stock_name), &(InDat.Stock_name), 8);

			memcpy(Year, &(InDat.Year), 4);
			memcpy(Month, &(InDat.Month), 2);
			memcpy(Day, &(InDat.Day), 2);

			int year = atoi(Year); 
			OutDat.date = atoi(Year)*372 + atoi(Month)*31 + atoi(Day) - 372 - 31; 
			OutDat.price = InDat.vwap;
			OutDat.volume = InDat.volume;
			OutDat.f2 = InDat.f2;

			write_struct(OutDat, StreamMap);

		}
	}
	else 
	{
		std::cout << "Could not read Input.txt file" << std::endl;
	}

	InFile.close();
	close_files(StreamMap);

	//system("pause");
	return 0;
}

