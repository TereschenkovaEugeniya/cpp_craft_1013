#ifndef _TASK4_6_SOLUTION_H_
#define _TASK4_6_SOLUTION_H_

#include <vector>
#include <string>
#include <map>

namespace task4_6
{
	typedef std::vector< std::string > lines;

	class solution
	{
	private :
		enum Error
		{
			NON_DEFINED,
			REDEFINITION,
			INVALID_EXP,
			ZERO_DIVISION
		};

		mutable std::map< const std::string, int > variables_;

		
		int evaluate( const char, int, int );
		std::string error_message( Error, int, char );
		void evaluate( std::string );
		void trim( std::string& );
	public:
		explicit solution( const lines& calulator_lines );
		int result_for( const std::string& key ) const;
	};
}

#endif // _TASK4_5_SOLUTION_H_