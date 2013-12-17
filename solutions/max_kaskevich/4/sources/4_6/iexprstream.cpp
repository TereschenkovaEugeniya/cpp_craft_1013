#include "iexprstream.h"


using namespace boost::spirit::standard;
using namespace boost::spirit::qi;


task4_6::iexprstream::iexprstream( const std::string& str ) :
    it_( str.cbegin() ),
    end_ ( str.cend() ),
    fail_( false )
{
}

task4_6::iexprstream::~iexprstream( void )
{
}

static const rule< std::string::const_iterator, std::string(), space_type > var_name = +char_( "A-Z" );

static const rule< std::string::const_iterator, std::string(), space_type > double_str
    = +char_( "0-9" ) >> -( char_( '.' ) >> +char_( "0-9" ) );

static const rule< std::string::const_iterator, std::string(), space_type > math_operator
    = char_( "+*/()-" );

static const rule< std::string::const_iterator, std::string(), space_type > token
    = double_str | math_operator | var_name;

task4_6::iexprstream& task4_6::operator >> ( task4_6::iexprstream& input, std::string& str )
{
    str.clear();
    phrase_parse( input.it_, input.end_, token, space, str );
    if( str.empty() )
    {
        input.fail_ = true;
    }
    return input;
}

task4_6::iexprstream::operator bool()
{
    return !fail_;
}