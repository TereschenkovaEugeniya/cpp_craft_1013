#include "solution.h"

task4_6::solution::solution( const lines& calculator_lines )
{
	line_index = 0;
	for( lines::const_iterator it = calculator_lines.begin(); it!= calculator_lines.end(); ++it, ++line_index )
		process_line( *it );
}

void task4_6::solution::process_line( std::string line )
{
	int eq_pos = line.find("=");
	const std::string& value_name = line.substr( 0, eq_pos-1 );
	line = line.substr( eq_pos + 2 );

	if( value.count( value_name ) )
		throw std::logic_error( "such variable '"+value_name+"' already exists ("+boost::lexical_cast< std::string >( line_index )+")" );

	std::stack< char > operations;
	std::stack< double > numbers;
	double result = 0;

	for( size_t i = 0; i < line.length(); i++)
		if( line[i] != ' ' )
			if( line[i] == '(' )
				operations.push('(');
			else if( line[i] ==')' )
			{
				while( !operations.empty() && operations.top() != '(' )
				{
					process_operation( numbers, operations.top() );
					operations.pop();
				}

				if( operations.empty() )
					throw std::logic_error("not correct expression at "+boost::lexical_cast< std::string >( line_index )+" line");
				
				operations.pop();
			}
			else if( is_operation( line[i] ) )
			{
				while( !operations.empty() && get_priority( operations.top() ) >= get_priority( line[i] ) )
				{
					process_operation( numbers, operations.top() );
					operations.pop();
				}
				operations.push( line[i] ); 
			}
			else
			{
				std::string operand;
				while (i < line.length() && line[i]!=' ' && !is_operation(line[i]) && line[i]!='(' && line[i]!=')')
					operand += line[i++];
				--i;
				double to_push;
				parse_variable ( operand, to_push );
				numbers.push ( to_push );
			}

	while ( !operations.empty() )
	{
		process_operation( numbers, operations.top() );
		operations.pop();
	}

	result = numbers.top();
	value[ value_name ] = result;
}

void task4_6::solution::parse_variable( const std::string name, double & variable)
{

	try
	{
		variable = boost::lexical_cast< double >( name );
	}
	catch( boost::bad_lexical_cast & )
	{
		if( !value.count( name ) )
			throw std::logic_error( "'"+name+"' variable not defined at line "+boost::lexical_cast< std::string >( line_index ) );
		variable = value.at( name );
	}

}

void task4_6::solution::process_operation( std::stack< double >& numbers, const char op)
{
	double l, r;

	if( op == '(' || op == ')' || numbers.size() < 2)
		throw std::logic_error("not correct expression at "+boost::lexical_cast< std::string >( line_index )+" line");

	r = numbers.top();
	numbers.pop();
	l = numbers.top();
	numbers.pop();

	switch( op )
	{
	case '+':
		numbers.push( l+r );
		break;
	case '-':
		numbers.push( l-r );
		break;
	case '*':
		numbers.push( l*r );
		break;
	case '/':
		if ( fabs(r)<=1e-9 )
			throw std::logic_error( "zero div " + boost::lexical_cast< std::string >( line_index ) );
		numbers.push( l/r );
		break;
	}

}

int task4_6::solution::result_for( const std::string& key ) const
{
	return static_cast<int>( floor( value.at( key ) + 0.5 ) );
}

bool task4_6::solution::is_operation( const char c ) const
{
	return ( c == '+' || c == '-' || c == '*' || c == '/');
}

int task4_6::solution::get_priority( const char operation ) const 
{
	if( operation == '+' || operation == '-' )
		return 1;
	else if( operation == '(' || operation == ')')
		return -1;
	return 2; 
}
