#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include <set>
#include <vector>
#include <algorithm>

#include "common.h"
#include "tools.h"

int main() {
	std::cout << "*Homework 24*" << std::endl;

	dataVector_t dataVector;

	std::cout << "read input file" << std::endl;

	if ( !readDataVectorFromFile(dataVector, SOURCE_DIR "/sources/24/input.txt") ){
		return 1;
	}

	std::cout << "\nwrite output file" << std::endl;

	for (dataVector_t::const_iterator it = dataVector.begin(); it != dataVector.end(); ++it){
		std::cout << "tp:" << (*it).type << "\ttm:" << (*it).time <<
				"\tln:" << (*it).msg.length() << "\t[" << (*it).msg << "]" << std::endl;
	}

	if (!writeDataVectorToFile(dataVector,  SOURCE_DIR "/sources/24/output.txt")){
		return 1;
	}

	std::cout << "---done---" << std::endl;

	return 0;
}


