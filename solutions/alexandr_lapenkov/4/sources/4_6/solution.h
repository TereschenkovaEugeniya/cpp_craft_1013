#ifndef _TASK4_6_SOLUTION_H_
#define _TASK4_6_SOLUTION_H_

#include <vector>
#include <string>
#include <exception>
#include <stack>
#include <utility>

#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

namespace task4_6
{
	typedef std::vector< std::string > lines;

	class solution
	{
	private:
		boost::unordered_map< std::string, double > value;
		int line_index;
		void process_line( std::string );
		void process_operation( std::stack< double >& numbers, const char op);
		void parse_variable( const std::string name, double & variable);
		bool is_operation( const char ) const;
		int get_priority( const char ) const;
		
	public:
		explicit solution( const lines& calculator_lines );
		int result_for( const std::string& key ) const;
	};
}

#endif // _TASK4_5_SOLUTION_H_