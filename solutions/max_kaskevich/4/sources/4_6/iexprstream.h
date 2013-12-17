#pragma once

#include <boost/spirit/include/qi.hpp>

namespace task4_6
{
    // its too lazy realize own basic_iexprstream or inherit stringstream :(
    class iexprstream 
    {   
        std::string::const_iterator it_;
        std::string::const_iterator end_;
        bool fail_;

        friend iexprstream& operator >> ( iexprstream& input, std::string& str );
    public:
        explicit iexprstream( const std::string& str );
        ~iexprstream( void );
        operator bool();
    };

    iexprstream& operator >> ( iexprstream& input, std::string& str );

} // namespace task4_6
