#include "b_message.h"

task4_4::b_message::b_message( std::istream& inp )
{

	
	inp >> length_;
	if ( inp.eof() )
		throw std::logic_error("bad input stream, b_message cannot be read");

	content_ = new char[ length_ ];

	inp.seekg(int(inp.tellg()) + 1);
	inp.read(content_, length_);
	

	if ( inp.eof() )
	{
		delete [] content_;
		throw std::logic_error("incorrect message in input stream, b_message cannot be read");	
	}

	out_str << "b_message(" << length_ << '|' << std::string(content_,length_) << ')';
}

task4_4::message_ptr task4_4::b_message::create_message( std::istream& inp )
{
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
	return out_str.str();
}
