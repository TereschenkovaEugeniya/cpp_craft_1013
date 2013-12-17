#include "b_message.h"

#include <sstream>

task4_4::b_message::b_message( std::istream& inp ):
	length_(0)
{
	inp >> length_;
	if(inp.eof()){
		throw std::logic_error("bad input stream, a_message cannot be readed");
	}

	content_ = new char[ length_ ];
	inp.read( content_, 1 );
	inp.read( content_, length_ );

	if( (size_t)inp.gcount() < length_){
		throw std::logic_error("bad input stream, not enough data");
	}
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
	std::stringstream sBuff;
	sBuff << "b_message(" << length_ << "|" << std::string(content_, length_) << ")";
	return sBuff.str();
}
