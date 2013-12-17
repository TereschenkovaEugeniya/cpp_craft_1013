#include "tools.h"

#include <iostream>
#include <fstream>
#include <set>


bool readDataVectorFromFile(dataVector_t& vec, const std::string& filename){
	std::ifstream iFile;
	iFile.open( filename.c_str(), std::ios::in | std::ios::binary );

	if ( !iFile.is_open() ) {
		std::cout << "cannot open input bin file" << std::endl;
		return false;
	}

	uint32_t tmp[] = {1u, 2u, 3u, 4u};
	static const std::set<uint32_t> acceptableOperations(&tmp[0], &tmp[0]+ 4);

	std::string sBuff;
	uint32_t tp;
	uint32_t tm;
	uint32_t ln;
	uint32_t maxTime = 0;
	bool isRecordValid;
	dataStruct_t dataRecord;

	while (!iFile.eof()) {
		isRecordValid = true;

		iFile.read((char*) &tp, sizeof tp);
		iFile.read((char*) &tm, sizeof tm);
		iFile.read((char*) &ln, sizeof ln);

		char* buff = new char[ln];
		iFile.read(buff, ln);
		sBuff = buff;
		sBuff = sBuff.substr(0, ln);
		delete[] buff;

		if ( (!acceptableOperations.count(tp)) ||
				(tm + 2 <= maxTime) ||
				(iFile.eof()) ){
			isRecordValid = false;
			std::cout << "FILTERED ";
		}

		if ( maxTime < tm ){
			maxTime = tm;
		}

		std::cout << "tp:" << tp << "\ttm:" << tm <<
				"\tln:" << ln << "\t[" << sBuff << "]" << std::endl;

		if (isRecordValid){
			dataRecord.type = tp;
			dataRecord.time = tm;
			dataRecord.msg = sBuff;
			vec.push_back(dataRecord);
		}
	}

	iFile.close();

	return true;
}

bool writeDataVectorToFile(dataVector_t& vec, const std::string& filename){
	std::ofstream oFile(filename.c_str(), std::ios::out | std::ios::binary);
	if (!oFile.is_open()){
		std::cout << "cannot open output file" << std::endl;
		return false;
	}

	size_t x;
	for (dataVector_t::const_iterator it = vec.begin(); it != vec.end(); ++it){
		oFile.write( (char*) &(*it).type, sizeof (*it).type );
		oFile.write( (char*) &(*it).time, sizeof (*it).time );
		x = (*it).msg.length();
		oFile.write( (char*) &x, sizeof x );
		oFile.write( (char*) (*it).msg.c_str(), x );
	}

	oFile.close();

	return true;
}
