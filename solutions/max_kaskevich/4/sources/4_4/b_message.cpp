#include "b_message.h"
#include <boost/format.hpp>

task4_4::b_message::b_message( std::istream& inp ) :
    length_(0)
{
    
	inp >> length_;
	content_.reset( new char[ length_ ] );
    inp.ignore();
	inp.read( content_.get(), length_ );
    if ( !inp )
        throw std::logic_error( "bad input stream, a_message cannot be readed" );
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
	return  ( boost::format( "b_message(%1%|%2%)" ) % length_ %
        std::string( content_.get(), length_ ) ).str();
}
