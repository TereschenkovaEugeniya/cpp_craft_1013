#include "buf.h"
#include <string>
// if no messages is of proper size than output file will be empty
// output files contains only saved in buffer types

int main(int argc, char** argv)
{
	char * num = "_253";
	std::string s = BINARY_DIR "/input" + string(num) + ".txt";
	std::cout << s;
	std::string out = BINARY_DIR "/output" + string(num) + ".txt";
	buf b(s, out);
	b.createOutput();
	return 0;
}