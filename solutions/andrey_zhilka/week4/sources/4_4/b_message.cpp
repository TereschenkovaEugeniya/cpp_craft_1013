#include "b_message.h"
#include <sstream>

task4_4::b_message::b_message( std::istream& inp )
{
	inp >> length_;
	content_ = new char[ length_ + 1 ];

	char next_char = inp.peek();
	
	while ( next_char > '9' || next_char < '0' )
	{
		inp.get();
		next_char = inp.peek();
	}

	inp.read( content_, length_ );
	if ( inp.eof() )
	{
		delete [] content_;
		throw std::logic_error( "actual message length is less" );
	}
	content_[ length_ ] = 0;
}

task4_4::message_ptr task4_4::b_message::create_message( std::istream& inp )
{
	inp.peek();
	if ( inp.eof() )
	{
		throw std::logic_error("bad input stream, b_message can not be created");
	}
	return message_ptr( new b_message( inp ) );
}

task4_4::b_message::~b_message()
{
	delete [] content_;
}

const char task4_4::b_message::type() const
{
	return 'B';
}
const std::string task4_4::b_message::str() const
{
	std::stringstream str;
	str << "b_message(" << length_ << "|" << content_ << ")";

	return str.str();
}
