#include "Deals.h"
#include <string>

int main(int argc, char** argv)
{
	char * num = "_242";
	const string searchPath = BINARY_DIR + string("/");
	string s =  searchPath + "input" + string(num) + ".txt";
	string out_f = searchPath + "output" + string(num) + ".txt";
	Deals d(s, out_f);
	d.createOutput();
	return 0;
}