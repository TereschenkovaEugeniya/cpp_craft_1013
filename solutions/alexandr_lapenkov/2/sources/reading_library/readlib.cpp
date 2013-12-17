#include "readlib.h"

binreader::binreader(): error(0){}

binreader::binreader(const char* filename)
{
	binreader();
	open(filename);
}

char binreader::get_char()
{
	if(!good())return 0;
	char res=0;

	if(!in.read(reinterpret_cast<char*>(&res), sizeof(res)))
		error = 1;

	return res;
}

double binreader::get_double()
{
	if(!good())return 0;
	double res;
	if(!in.read(reinterpret_cast<char*>(&res),sizeof(double)))
		error = 1;
	return res;
}

std::string binreader::get_line(int len)
{
	if(!good())return "";
	std::string res;
	char *c;

	c = new char [len+1];

	if(!in.read(c,sizeof(char)*len))
		error = 1;

	res.assign(c,c+len);
	delete []c;
	return res;
}

unsigned binreader::get_unsigned()
{
	if(!good())return 0;
	unsigned res;
	if(!in.read(reinterpret_cast<char*>(&res),sizeof(unsigned)))
		error = 1;
	return res;
}

bool binreader::good()
{
	return !error;
}

void binreader::open(const char *filename)
{
	in.open(filename,std::ios_base::in|std::ios_base::binary);
}

void binreader::close()
{
	in.close();
}

bool binreader::is_open()
{
	return in.is_open();
}

binreader& operator>>(binreader& in, char& c)
{
	c = in.get_char();
	return in;
}

binreader& operator>>(binreader& in, unsigned& c)
{
	c = in.get_unsigned();
	return in;
}

binreader& operator>>(binreader& in, double& c)
{
	c = in.get_double();
	return in;
}