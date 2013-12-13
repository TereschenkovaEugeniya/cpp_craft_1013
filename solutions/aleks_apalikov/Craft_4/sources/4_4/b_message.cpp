#include "b_message.h"
#include <sstream>
#include <stdexcept>


task4_4::b_message::b_message( std::istream& inp )
{
	length_ = 0;
	inp.clear();
	if(!inp.eof())
		inp >> length_;
	if(inp.eof())
		throw std::logic_error("Empty input str, b_message cannot be read");
	char c;
	if(length_ > 0)
	{
		content_ = new char[ length_ + 1];
	}
	std::stringstream ss;
	for(size_t i = 0; i < length_; i++)
	{
		inp >> c;
		if(inp.eof())
		{
			delete[] content_;
			throw std::logic_error("bad input stream, b_message cannot be read");
		}
		ss << c;
	}
	ss.read( content_, length_ );
	content_[ length_ ] = 0;
	if (ss.fail())
		throw std::logic_error("bad input stream, b_message cannot be read");
	
}

task4_4::message_ptr task4_4::b_message::create_message( std::istream& inp )
{
	return message_ptr( new b_message( inp ) );
}

task4_4::b_message::~b_message()
{
	if(length_ > 0)
		delete [] content_;
}

const char task4_4::b_message::type() const
{
	return 'B';
}
const std::string task4_4::b_message::str() const
{
	std::string st(content_, length_);
	std::stringstream ss;
	ss << "b_message(" << length_ << "|" << st << ")";
	std::string st2 = ss.str();
	return st2;
}
