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
        const lines& lines_;
        typedef  std::map< const std::string, double > vars_map;
        vars_map vars_;

        void calculate();
        double calculate_rpn( const std::string& rpn_expression );
	public:
		explicit solution( const lines& calulator_lines );
		int result_for( const std::string& key ) const;
	};

    std::string to_rpn( const std::string& expression );
}

#endif // _TASK4_6_SOLUTION_H_
