#include <iostream>
#include <fstream>
#include <stdint.h>
#include <string>
#include <stdlib.h>

uint32_t ymd2days(const std::string& ymd){
	static const unsigned int daysInYear = 372;
	static const unsigned int daysInMonth = 31;

	unsigned int year;
	unsigned int month;
	unsigned int day;

	year = atoi(ymd.substr(0, 4).c_str());
	month = atoi(ymd.substr(4, 2).c_str());
	day = atoi(ymd.substr(6, 2).c_str());

	return ( (year - 1) * daysInYear + (month - 1) * daysInMonth + day );
}

int main(int argc, char** argv) {
	std::cout << "*Hometask 26*" << std::endl;

	std::ifstream iFile;
	iFile.open( SOURCE_DIR "/sources/26/input.txt",
			std::ios::in | std::ios::binary);

	std::ofstream oFile;
	oFile.open( SOURCE_DIR "/sources/26/output.txt",
			std::ios::out | std::ios::binary);

	if (!iFile.is_open() || !oFile.is_open()) {
		std::cout << "cannot open bin files" << std::endl;
		return 1;
	}

	static const uint32_t stringSizeInput = 8;
	static const uint32_t stringSizeOutput = 9;
	static const uint32_t dateLength = 8;
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

	char buff[stringSizeInput];

	while (!iFile.eof()) {
		//input
		iFile.read( (char*)&buff, stringSizeInput);
		stockName = buff;
		stockName = stockName.substr(0, stringSizeInput);

		iFile.read( (char*)&buff, dateLength);
		date = buff;
		date = date.substr(0, dateLength);

		iFile.read((char*) &price, sizeof price);
		iFile.read((char*) &vwap, sizeof vwap);
		iFile.read((char*) &volume, sizeof volume);
		iFile.read((char*) &f1, sizeof f1);
		iFile.read((char*) &t1, sizeof t1);
		iFile.read((char*) &f2, sizeof f2);
		iFile.read((char*) &f3, sizeof f3);
		iFile.read((char*) &f4, sizeof f4);

		if (iFile.eof()){
			continue;
		}

		std::cout << "IN:\tsn[" << stockName << "]\tdt[" << date
				<< "]\tpr:" << price
				<< "\tvw:" << vwap
				<< "\tvl:" << volume
				<< "\tf1:" << f1
				<< "\tt1:" << t1
				<< "\tf2:" << f2
				<< "\tf3:" << f3
				<< "\tf4:" << f4
				<< std::endl;

		//output
		stockName.append("\0\0\0\0\0\0\0\0\0\0");
		stockName = stockName.substr(0, stringSizeOutput);
		std::cout << "OUT\t[" << stockName.c_str() << "]\t";

		uint32_t days = ymd2days(date);
		std::cout << ymd2days(date) << "\t";
		std::cout << vwap << "\t" << volume << "\t" << f2 << std::endl;

		oFile.write( (char*)stockName.c_str(), stringSizeOutput );
		oFile.write( (char*)&days, sizeof days);
		oFile.write( (char*)&vwap, sizeof vwap);
		oFile.write( (char*)&volume, sizeof volume);
		oFile.write( (char*)&f2, sizeof f2);
	}

	iFile.close();
	oFile.close();

	std::cout << "\n--EOF--" << std::endl;

	return 0;
}
