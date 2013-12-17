/*
 *       Filename: task13.cpp
 *    Description: Solution for Task # 1.3 Reversed words
 */

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cwctype>

static const std::string TEST_DIR(SOURCE_DIR "/tests/");
static const std::string OUTPUT_FILE (TEST_DIR + "output.txt");
static const std::string INPUT_FILE_DEFAULT (TEST_DIR + "input.txt");

bool is_delim(const wchar_t c)
{	
	return c == ' ' || c == '\\' || c == '-';
}

const std::wstring tolower_and_del_delims(const std::wstring& str, const bool reverse)
{
	std::wstring ret_str;
	if(reverse) {
		remove_copy_if(str.rbegin(), str.rend(), std::back_inserter(ret_str), is_delim);	
	} else {
		remove_copy_if(str.begin(), str.end(), std::back_inserter(ret_str), is_delim);	
	}	
	std::transform(ret_str.begin(), ret_str.end(), ret_str.begin(), std::towlower);
	return ret_str;
}

int main(int argc, char **argv)
{
	std::wifstream in(INPUT_FILE_DEFAULT.c_str());
	if(!in.good()) {
		std::cout << "Error opening input file " << INPUT_FILE_DEFAULT << std::endl;
		exit(1);
	}
	std::wofstream out(OUTPUT_FILE.c_str());
	if(!out.good()) {
		std::cout << "Error opening ouput file " << OUTPUT_FILE << std::endl;
		exit(1);
	}
	std::cout << "Input file: " << INPUT_FILE_DEFAULT << std::endl;
	std::wstring line;
	getline(in, line);
	std::wstring firstline = tolower_and_del_delims(line, true);
	while ( getline(in, line) ) {
		if(!line.empty()) {			
			bool found = ( firstline.find( tolower_and_del_delims(line, false) ) != std::wstring::npos );
			out << ((found) ? "YES" : "NO") << std::endl;
		}
	}
	std::cout << "Output file: " << OUTPUT_FILE << std::endl;
	return 0;
}
