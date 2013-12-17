#include "b_message.h"

#include <boost/format.hpp>

task4_4::b_message::b_message( std::istream& inp )
{
    size_t length_ = 0;
	inp >> length_;
    if (!length_)
    {
        throw std::logic_error("bad input stream, nothing to read");
    }

    char must_be_space;
    inp.get(must_be_space);
    if (must_be_space != ' ')
    {
        throw std::logic_error("bad input stream, failed string format");
    }

    content_.resize(length_);
	inp.read( &content_[0], length_ );
    if (inp.gcount() != length_)
    {
        throw std::logic_error("bad input stream, not full message to read");
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
	return boost::str(boost::format("b_message(%1%|%2%)") % content_.size() % std::string(&content_[0], content_.size()));
}
