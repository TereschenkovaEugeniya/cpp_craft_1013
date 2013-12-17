#include "binreader.h"

binreader::binreader( std::stringstream& in )
	:error( 0 )
{
	this->in << in.rdbuf();
}

char binreader::get()
{
	if(!good())return 0;
	char res=0;

	if(!in.read(reinterpret_cast<char*>(&res), sizeof(res)))
		error = 1;
	if(!res)
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

	for(int i = 0; i<len&&c[i]; ++i)
		res+=c[i];
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

void binreader::skip( const size_t bytes_cnt )
{
	char * buffer = new char [ bytes_cnt ];
	if( !in.read( buffer, bytes_cnt ) )
		error = 1;
	delete [] buffer;
}

binreader& operator>>(binreader& in, char& c)
{
	c = in.get();
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