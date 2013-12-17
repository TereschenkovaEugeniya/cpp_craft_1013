#include <string.h>
#include <iostream>
#include <fstream>

class binreader
{
	std::ifstream in;
	bool error;

	friend binreader& operator>>(binreader&, char&);
	friend binreader& operator>>(binreader&, unsigned&);
	friend binreader& operator>>(binreader&, double&);


public:

	binreader();
	binreader(const char *);
	
	char get_char();
	unsigned get_unsigned();
	std::string get_line(int);
	double get_double();
	bool good();
	void open(const char *);
	void close();
	bool is_open();
};