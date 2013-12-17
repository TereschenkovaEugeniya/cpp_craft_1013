#include "solution.h"
#include <sstream>
#include <algorithm>

task4_6::solution::solution( const lines& calulator_lines )
{
}

int task4_6::solution::result_for( const std::string& key ) const
{
	return variables_[key];
}

int task4_6::solution::evaluate( const char operation, int operand1, int operand2 )
{
	int result;
	switch( operation )
	{
	case '*':
		result = operand1 * operand2;
	case '/':
		if ( operand2 == 0 )
		{
			throw new std::logic_error("");
		}
		result = operand1 / operand2;
	case '+':
		result = operand1 + operand2;
	case '-':
		result = operand1 - operand2;
	}	

	return result;
}

std::string task4_6::solution::error_message(task4_6::solution::Error code, int index, char var_name = '-')
{
	std::stringstream message;

	switch( code )
	{
	case INVALID_EXP :
		message << "not correct expression at " << index << "line";
		break;
	case NON_DEFINED :
		message << "'" << var_name << "' variable not defined at line " << index;
		break;
	case REDEFINITION :
		message << "such variable '" << var_name << "' already exists";
		break;
	case ZERO_DIVISION :
		message << "zero div " << index;
		break;
	default :
		message << "undefined exception at line " << index;
	}

	return message.str();
}

void task4_6::solution::trim( std::string& source )
{
	source.erase( std::remove( source.begin(), source.end(), ' '), source.end() );
}