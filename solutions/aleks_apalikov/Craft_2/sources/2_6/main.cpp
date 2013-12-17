#include "2_6/Datafeed.h"
#include <string>

int main(int argc, char** argv)
{
	char * num = "_261";
	std::string in_file = BINARY_DIR "/input" + string(num) + ".txt";
	std::string out_file = BINARY_DIR "/output" + string(num) + ".txt";
	Datafeed df(in_file, out_file);
	df.createOutput();
	return 0;
}