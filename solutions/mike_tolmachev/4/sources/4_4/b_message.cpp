#include "b_message.h"

task4_4::b_message::b_message( std::istream& inp )
{
	inp >> length_;
    if ( inp.fail() )
    {
		throw std::logic_error("bad input stream, a_message cannot be readed");
    }
    else
    {
        inp.ignore(1);
	    content_ = new char[ length_ ];
	    inp.read( content_, length_ );
        if ( inp.fail() )
        {
            delete [] content_;
		    throw std::logic_error("bad input stream, a_message cannot be readed");
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
    std::ostringstream ostr;
    ostr << "b_message(" << length_ << "|" << std::string(content_, length_) << ")";

    return ostr.str();
}
