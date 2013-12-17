#include <stdint.h>
#include <iostream>
#include <fstream>


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

int main()
{
	std::fstream InFile;
	std::fstream OutFile;

	struct InDataStruct InDat;
	struct OutDataStruct OutDat;
	char Year[4];
	char Month[2];
	char Day[2];


	InFile.open(BINARY_DIR"/Input.txt",std::ios_base::binary|std::ios_base::in);
	OutFile.open(BINARY_DIR"/Output.txt", std::ios_base::binary|std::ios_base::out|std::ios_base::trunc);

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

			OutFile.write((char*)&OutDat, sizeof(OutDat));
		}
	}
	else 
	{
		std::cout << "Could not read Input.txt file" << std::endl;
	}

	InFile.close();
	OutFile.close();

	//system("pause");
	return 0;
}
