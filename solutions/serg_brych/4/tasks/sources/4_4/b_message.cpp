#include "b_message.h"

task4_4::b_message::b_message( std::istream& inp )
{
	inp >> length_;
	if ( inp.eof() )
		throw std::logic_error("bad input stream, a_message cannot be readed");
	
	++length_;
	content_.reset(new char[ length_ ]);
	inp.read( content_.get(), length_ );
	if ( inp.eof() )
	{
		throw std::logic_error("bad input stream, a_message cannot be readed");
	}
}

task4_4::message_ptr task4_4::b_message::create_message( std::istream& inp )
{
	return message_ptr( new b_message( inp ) );
}

task4_4::b_message::~b_message()
{
}

const char task4_4::b_message::type() const
{
	return 'B';
}
const std::string task4_4::b_message::str() const
{
	std::stringstream ss;
	ss << "b_message("  << length_ - 1 << "|"<< std::string(&content_[1], length_ - 1) << ")";

	return ss.str();
	
}
