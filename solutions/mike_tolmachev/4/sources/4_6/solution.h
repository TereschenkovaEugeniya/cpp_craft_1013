#ifndef _TASK4_6_SOLUTION_H_
#define _TASK4_6_SOLUTION_H_

#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <boost\format.hpp>

namespace task4_6
{
    enum TokenValue
    {
        END,
        NAME,
        NUMBER,
        PLUS='+',
        MINUS='-',
        MUL='*',
        DIV='/',
        ASSIGN='=',
        LP='(',
        RP=')'
    };

	typedef std::vector< std::string > lines;

	class solution
	{
	public:
		explicit solution( const lines& calulator_lines );
		int result_for( const std::string& key ) const;
    private:
        TokenValue GetToken();
        double Expr(bool from_rec);
        double Term();
        double Prim();

        TokenValue cur_tok;
        std::map<std::string, int> names;

        double number_val;
        std::string name_val;
        std::istringstream sstr;
	};
}

#endif // _TASK4_5_SOLUTION_H_