#include <stock_data.h>
#include "Trade.h"
#include <sstream>

int Trade::write( ofstream& o )
{
	binary_reader::write_binary(o, typ);
	binary_reader::write_binary(o, time);
	binary_reader::write_binary(o, len);
	string::iterator ch = str.begin();
	for(size_t i = 0; i < len; i++, ch++)
	{
		binary_reader::write_binary(o, *ch);
	}
	return 0;
}

int Trade::read( ifstream& inp )
{
	if( inp.peek() == EOF )
	{
		return -1;
	}
	binary_reader::read_binary(inp, typ);
	binary_reader::read_binary(inp, time);
	binary_reader::read_binary(inp, len);
	stringstream s;
	char ch = ' ';
	for(size_t i = 0; i < len; i++)
	{
		binary_reader::read_binary(inp, ch);
		s << ch;
	}
	str = s.str();
	return 0;
}
