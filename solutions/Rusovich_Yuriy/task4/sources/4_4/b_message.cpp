#include "b_message.h"

#include <sstream>

task4_4::b_message::b_message( std::istream& inp )
{
	inp >> length_;
	if (inp.eof())
	{
		throw std::logic_error("bad input file");
	} else
	{
		content_ = new char[ length_ ];
		inp.get();
		inp.read( content_, length_ );
		if (inp.gcount() != length_)
		{
			delete [] content_;
			throw std::logic_error("bad count sequence");
		}
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
	std::stringstream ss;
	ss << "b_message" << "(" << length_ << "|" << std::string(content_, length_) << ")";
	return ss.str();
}
