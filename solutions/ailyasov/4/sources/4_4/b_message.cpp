#include <sstream>
#include <cstring>
#include <stdexcept>

#include <boost/shared_array.hpp>

#include "b_message.h" 

task4_4::b_message::b_message( std::istream& inp )
{
    inp >> length_;
    if(inp.eof() || !inp.good())
        throw std::logic_error("Empty input");    
	content_ = boost::shared_array<char>(new char[length_+1]);
    content_.get()[length_] = '\0';	
    inp.get();
    inp.read( content_.get(), length_);
    if(inp.gcount() != length_  )
        throw std::logic_error("Wrong length");
}

task4_4::message_ptr task4_4::b_message::create_message( std::istream& inp )
{
    return message_ptr( new b_message( inp ) );
}

task4_4::b_message::~b_message() { }

const char task4_4::b_message::type() const
{
    return 'B';
}
const std::string task4_4::b_message::str() const
{
    std::stringstream ss;
    ss << "b_message(" << length_ << "|" << content_.get() << ")";
    return ss.str(); 
}
