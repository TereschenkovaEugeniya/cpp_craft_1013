/*
 *       Filename:  task14.cpp
 *    Description:  Solution for task # 1.4 Rational Numbers
 */

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

static const std::string TEST_DIR(SOURCE_DIR "/tests/");
static const std::string OUTPUT_FILE (TEST_DIR + "output.txt");
static const std::string INPUT_FILE (TEST_DIR + "input2.txt");

static const int PRECISION = 1e4;

inline long long int round_and_intcast(const double f)
{
    return static_cast<long long int>(f * PRECISION);
}

int main(int argc, char **argv)
{
	std::wifstream in(INPUT_FILE.c_str());
	if(!in.is_open()) {
		std::cout << "Error opening input file " << INPUT_FILE << std::endl;
		exit(1);
	}
	std::wofstream out(OUTPUT_FILE.c_str());
	if(!out.is_open()) {
		std::cout << "Error opening ouput file " << OUTPUT_FILE << std::endl;
		exit(1);
	}
	std::cout << "Input file: " << INPUT_FILE << std::endl;
	int N;
	std::vector<long long int> codes;
	double f;
	in >> N;
	for(int i = 0; i < N; i++) {			
		in >> f;
		codes.push_back(round_and_intcast(f));
	}
	in >> f;
	while(in.good()) {
		bool found = ( std::find(codes.begin(), codes.end(), round_and_intcast(f) ) != codes.end());
		out << ((found) ? "YES" : "NO") << std::endl;		
		in >> f;
	}

	std::cout << "Output file: " << OUTPUT_FILE << std::endl;
	return 0;
}
