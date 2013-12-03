#ifndef _BINREADER_H_
#define _BINREADER_H_

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

class binreader
{
	std::stringstream in;
	bool error;

	friend binreader& operator>>(binreader&, char&);
	friend binreader& operator>>(binreader&, unsigned&);
	friend binreader& operator>>(binreader&, double&);

public:

	binreader( std::stringstream& in );
	
	char get();
	unsigned get_unsigned();
	std::string get_line(int);
	double get_double();
	bool good();
	void skip( const size_t bytes_cnt );
};

#endif //_BINREADER_H_