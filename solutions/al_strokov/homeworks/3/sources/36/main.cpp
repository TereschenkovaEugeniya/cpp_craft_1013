#include "binFileReader.h"
#include "binFileWriter.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>


typedef std::map<std::string, std::vector<dataStruct2_t> > splittedMessages_t;

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

int main(int argc, char** argv){
    std::cout << "*Homework 36*" << std::endl;

    std::string filenamei = SOURCE_DIR "/sources/36/input.txt";

    binFileReader bfr(filenamei);

    if(!bfr.fileOk){
    	std::cout << "cannot open input file" << std::endl;
    	return 1;
    }

    dataStruct2_t ds2;
    splittedMessages_t splitted;

    while(bfr.readDataStruct(ds2)){
    	std::cout << ds2.stockName << "\t" << ds2.date << std::endl;

    	splitted[ds2.stockName].push_back(ds2);
    }

    std::stringstream ssFilenameo;
    binFileWriter bfw;

    for(splittedMessages_t::iterator it = splitted.begin(); it != splitted.end(); ++it){
    	ssFilenameo.str("");
    	ssFilenameo << SOURCE_DIR << "/sources/36/output_" << (*it).first << ".txt";
    	std::cout << "-->" << ssFilenameo.str() << std::endl;

    	bfw.open(ssFilenameo.str());

    	if(!bfw.fileOk){
    		std::cout << "cannot open file to output" << std::endl;
    		continue;
    	}

    	for(std::vector<dataStruct2_t>::iterator i = (*it).second.begin(); i != (*it).second.end(); ++i){
    		(*i).stockName.append("\0\0\0\0\0\0\0\0\0\0");
    		(*i).stockName = (*i).stockName.substr(0, dataStruct2_t::stockNameOutputLength);
    		std::cout << "OUT\t[" << (*i).stockName.c_str() << "]\t";

    		uint32_t days = ymd2days((*i).date);
    		std::cout << ymd2days((*i).date) << "\t";
    		std::cout << (*i).vwap << "\t" << (*i).volume << "\t" << (*i).f2 << std::endl;

    		bfw.writeRawValue( (*i).stockName, dataStruct2_t::stockNameOutputLength );
    		bfw.writeRawValue(days);
    		bfw.writeRawValue((*i).vwap);
    		bfw.writeRawValue((*i).volume);
    		bfw.writeRawValue((*i).f2);
    	}
    	bfw.close();
    }

    std::cout << "...done" << std::endl;
    return 0;
}
