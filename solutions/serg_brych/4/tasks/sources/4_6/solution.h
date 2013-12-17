#ifndef _TASK4_6_SOLUTION_H_
#define _TASK4_6_SOLUTION_H_

#include <vector>
#include <string>
#include <stack>
#include <cctype>
#include <stdint.h>
#include <sstream>
#include <stdexcept>
#include <map>

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp> 

namespace task4_6
{
	typedef std::vector< std::string > lines;
	typedef std::map<std::string, double> variables;
	class solution
	{
		const std::string get_pl_revers(const std::string& expression);
		double calc_pl_expr(const std::string& polish_expressions);

		bool is_number(char ch);
		bool is_number(const std::string& s);
		bool is_operation(char ch);
		uint32_t operation_priority(char ch);
		
		variables vars;

	public:
		explicit solution( const lines& calulator_lines );
		int result_for( const std::string& key ) const;
	};
}

#endif // _TASK4_6_SOLUTION_H_