#include <string.h>
#include <iostream>
#include <fstream>

class binwriter
{
	std::ofstream out;
	bool error;
	friend binwriter& operator << (binwriter&, const unsigned);
	friend binwriter& operator << (binwriter&, const double);
	friend binwriter& operator << (binwriter& out, const char * a);
	friend binwriter& operator << (binwriter& out, std::string a);

public:

	binwriter();
	binwriter(const char *);
	
	void write_unsigned(const unsigned);
	void write_double(const double);
	void write_line(const char *, int = -1);
	void open(const char *);
	void close();
	bool is_open();
	bool good();
};