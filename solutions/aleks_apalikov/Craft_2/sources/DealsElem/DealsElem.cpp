#include "DealsElem.h"
#include <string.h>
#include <stdlib.h>


DealsElem::DealsElem(void)
{
}

DealsElem::DealsElem( UINT32 type, UINT32 time, UINT32 len, char* str )
{
	Type = type;
	Time = time;
	Len = len;
	Msg = new char[strlen(str)+1];
	strcpy(Msg, str);

}

DealsElem::~DealsElem(void)
{
	if(Msg)
		delete[] Msg;
}

UINT32 write_uint32( ofstream& outs, UINT32 value)
{
	outs.write( reinterpret_cast< const char* >( &value ), sizeof( UINT32 ) );
	return value;
}
/*
{
	outs.write( reinterpret_cast< const char* >( &value ), sizeof( UINT32 ) );
	return value;
}
*/

void DealsElem::operator<<(ofstream& out)
{
	write_uint32(out, this->Type);
	write_uint32(out, this->Time);
	write_uint32(out, this->Len);
	write_str(out, this->Msg);
//	return out;
}

UINT32 read_uint32( fstream& ins)
{
	UINT32 value = 0;
	for (unsigned size = 0; size < sizeof( UINT32 ); ++size)
		value |= ins.get() << (8 * size);
	return value;
}
/*
{
	UINT32 value = 0;
	ins.read( reinterpret_cast< char* >( &value ), sizeof( UINT32 ) );
	return value;
} */

void write_double( ofstream& out, double rational ) 
{
	out.write( reinterpret_cast<char*>( &rational ), sizeof ( rational ) );
}

void write_str( ofstream& ofs, const char* str )
{
	for (unsigned size = 0; size < strlen(str); ++size)
	{
		if (str[size] == 0x01)
		{
			ofs.put(0x00);
		}
		else
			ofs.put(str[size]);
	}
	ofs.put( ( char ) 0);

}

ofstream& operator<<( ofstream& out, const DealsElem& de )
{
	write_uint32(out, de.Type);
	write_uint32(out, de.Time);
	write_uint32(out, de.Len);
	write_str(out, de.Msg);
	return out;
}

std::string read_str( fstream& ins )
{
	char c[9];
	for (size_t size = 0; size < 8; ++size)
	{
		ins.read(&(c[size]), 1);
		if(c[size] == 0x00)
		{
			c[size] = 0x01;
		}
	}
	c[8] = 0x00;

	string value = c;
	return value;
	
}

double read_double( fstream& ins )
{
	double rational = 0;
	ins.read( reinterpret_cast<char*>( &rational ), sizeof ( rational ) );
	return rational;
}

UINT32 read_date( fstream& ins )
{
	string s = read_str(ins);
	return atoi(s.c_str());

}

