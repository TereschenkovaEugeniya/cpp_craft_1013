#include "solution.h"
#include "iexprstream.h"

#include <sstream>
#include <stack>

#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>


task4_6::solution::solution( const lines& calulator_lines ) :
    lines_( calulator_lines )
{
    calculate();
}

int task4_6::solution::result_for( const std::string& key ) const
{
    // poor msvc 2012 not support std::round 8'(
	return static_cast<int>( vars_.at( key ) + ( vars_.at( key ) >= 0 ? .5 : -.5) );
}

static const std::string operators = "()+-*/";
static const std::map<char, uint32_t> priority = boost::assign::map_list_of
    ( '(', 0 ) ( ')', 0 ) ( '+', 1 ) ( '-', 1 ) ( '*', 2 ) ( '/', 2 );

// algorithm based on http://en.wikipedia.org/wiki/Reverse_Polish_notation
std::string task4_6::to_rpn( const std::string& expression )
{
    iexprstream input( expression );
    std::ostringstream output;

    std::string token;
    std::stack< char > rpnstack;

    while( input >> token )
    {
        if ( token == "(" || token == ")" )
        {
            // token is bracket
            if( token == ")" )
            {
                while( rpnstack.top() != '(' )
                {
                    output << rpnstack.top() << " ";
                    rpnstack.pop();
                    if( rpnstack.empty() )
                    {
                        throw std::logic_error( "not correct expression at %1% line" );
                    }
                }
                rpnstack.pop();
            }
            else
            {
                rpnstack.push( token[0] );
            }
        }
        else if( token.length() == 1 && operators.find( token[0] ) != token.npos )
        {
            // token is operator
            if( !rpnstack.empty() && priority.at( token[0] ) < priority.at( rpnstack.top() ) )
            {
                output << rpnstack.top() << " ";
                rpnstack.pop();
            }
            rpnstack.push( token[0] );
        }
        else
        {
            // token is var or number
            output << token << " ";
        }

    }
    while( !rpnstack.empty() )
    {
        output << rpnstack.top() << " ";
        rpnstack.pop();
    }
    return output.str();
}

double task4_6::solution::calculate_rpn( const std::string& rpn_expression )
{
     std::stack< double > calc_stack;
     std::istringstream input( rpn_expression );
     std::string token;
     vars_map::iterator it;
     while( input >> token )
     {
         if( token.length() == 1 && operators.find( token[0] ) != token.npos )
         {
             // token is operator
             double r = 0;

             if( calc_stack.empty() )
             {
                 throw std::logic_error( "not correct expression at %1% line" );
             }
             double y = calc_stack.top();
             calc_stack.pop();

             if( calc_stack.empty() )
             {
                 throw std::logic_error( "not correct expression at %1% line" );
             }
             double x = calc_stack.top();
             calc_stack.pop();
             switch ( token[0] )
             {
             case '+': r = x + y; break;
             case '-': r = x - y; break;
             case '*': r = x * y; break;
             case '/': r = x / y;
                 if ( y == 0 )
                 {
                     throw std::logic_error( "zero div (%1%)" );
                 }
                 break;
             default:
                 throw std::logic_error( "not correct expression at %1% line" );
             }
             calc_stack.push( r );
         }
         else if( ( it = vars_.find( token ) ) != vars_.end() )
         {
              // token is var
             calc_stack.push( it->second );
         }
         else
         {
             // maybe token is number
             try
             {
                 calc_stack.push( boost::lexical_cast<double>( token ) );
             }
             catch ( const boost::bad_lexical_cast& )
             {
             	 throw std::logic_error(
                     ( boost::format( "'%1%' variable not defined at line %2%" )
                     % token % "%1%" ).str() );
             }
         }
     }
     if( calc_stack.empty() )
     {
         throw std::logic_error( "not correct expression at %1% line" );
     }
     return calc_stack.top();
}

void task4_6::solution::calculate()
{
    std::vector< std::string > strs;
    for ( int i = 0; i < lines_.size(); ++i )
    {
        try
        {
            boost::split( strs, lines_[i], boost::is_any_of( "=" ) );
            if ( strs.size() > 2 )
            {
                throw std::logic_error( "not correct expression at %1% line" );
            }

            std::string var_name = boost::trim_copy( strs[0] );
            std::string expression = boost::trim_copy( strs[1] );
            if(vars_.find( var_name ) != vars_.end())
            {
                throw std::logic_error(
                    ( boost::format( "such variable '%1%' already exists (%2%)" )
                    % var_name % "%1%" ).str() ); 
            }

            vars_[ var_name ] = calculate_rpn( to_rpn( expression ) );
        }
        catch ( const std::logic_error& e )
        {
            throw std::logic_error( ( boost::format( e.what() ) % ( i + 1 ) ).str() );
        }
    }
}