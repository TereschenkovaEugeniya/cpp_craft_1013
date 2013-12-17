#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>
#include <string>


//transform given string to double with 4 digit precision
double strToDouble4(std::string& str) {
	if (str.find('.') != std::string::npos) {
		str += "00000";
		str.erase(str.find('.') + 5);
	} else {
		str += ".0";
	}

	return atof(str.c_str());
}


int main(int argc, char** argv) {
	std::cout << "*homework 14*" << std::endl;

	std::ifstream inputFile(SOURCE_DIR "/sources/14/input.txt");
	std::ofstream outputFile(SOURCE_DIR "/sources/14/output.txt");

	if ( !inputFile.is_open() || !outputFile.is_open()){
		std::cout << "cannot open files" << std::endl;
		return 1;
	}

	std::string line;
	std::string strBuff;
	unsigned int codesCount = 0;
	double password;
	std::set<double> setOfCodes;

	//get count of codes
	std::getline(inputFile, strBuff);
	codesCount = atoi(strBuff.c_str());
	std::cout << "codes count:\t" << codesCount << std::endl;

	//get codes to set
	for (unsigned int i = 0; i < codesCount; i++) {
		std::getline(inputFile, strBuff);
		std::cout << "code\t" << strBuff << std::endl;
		setOfCodes.insert(strToDouble4(strBuff));
	}

	//check if current password is in set of codes
	while (std::getline(inputFile, strBuff)) {
		password = strToDouble4(strBuff);
		std::cout << "pass\t" << password;

		if (setOfCodes.find(password) != setOfCodes.end()) {
			outputFile << "YES" << std::endl;
			std::cout << "\tYES" << std::endl;
		} else {
			outputFile << "NO" << std::endl;
			std::cout << "\tNO" << std::endl;
		}
	}

	inputFile.close();
	outputFile.close();

	return 0;
}
