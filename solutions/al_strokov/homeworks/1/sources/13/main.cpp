#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>


void prepareString( std::string& str ){
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
	str.erase(std::remove(str.begin(), str.end(), '-'), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\\'), str.end());
}

int main( int argc, char** argv ){
    std::cout << "*homework 13*" << std::endl;

    std::ifstream inputFile( SOURCE_DIR "/sources/13/input.txt" );
    std::ofstream outputFile( SOURCE_DIR "/sources/13/output.txt" );

    if ( !inputFile.is_open() || !outputFile.is_open()){
        std::cout << "cannot open files" << std::endl;
        return 1;
    }

    std::string line;
    std::string mainString;

    //process first line
    std::getline( inputFile, mainString );
    prepareString(mainString);

    //process keys
    while ( std::getline( inputFile, line ) ){
    	prepareString(line);
    	std::reverse(line.begin(), line.end());

    	if ( mainString.find(line) != std::string::npos ){
    		outputFile << "YES" << std::endl;
    	}else{
    		outputFile << "NO" << std::endl;
    	}
    }

    inputFile.close();
    outputFile.close();

    return 0;
}
