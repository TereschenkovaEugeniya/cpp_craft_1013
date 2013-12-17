#include "solution.h"

task4_6::solution::solution( const lines& calulator_lines )
{
    for (auto it = calulator_lines.begin(); it != calulator_lines.end(); ++it)
    {
        sstr.clear();
        sstr.str(*it);
		try
		{
			Expr(false);
		}
		catch(const std::string& str)
		{
			throw std::logic_error((boost::format(str) % std::distance(calulator_lines.begin(), it)).str());
		}
    }
}

int task4_6::solution::result_for( const std::string& key ) const
{
    int res = 0;
    auto it = names.find(key);
    if (it != names.end())
    {
        res = it->second;
    }

	return res;
}

task4_6::TokenValue task4_6::solution::GetToken()
{
    char ch = 0;

    do
    {
        sstr.get(ch);
        if (sstr.bad())
        {
            ch = 0;
        }
	} while (isspace(ch) && sstr.good());

	if (sstr.eof())
	{
		ch = 0;
	}

    switch (ch)
    {
    case 0:
        cur_tok = END;
        break;
    case '+':
    case '-':
    case '*':
    case '/': 
    case '=':
    case '(':
    case ')':
        cur_tok = TokenValue(ch);
        break;
    default:
        if (isdigit(ch))
        {
            sstr.putback(ch);
            sstr >> number_val;
            cur_tok = NUMBER;
        }
        else if (isalpha(ch))
        {
            name_val.clear();
            while(sstr.good() && isalnum(ch))
            {
                name_val.push_back(ch);
                sstr.get(ch);
            }
            sstr.putback(ch);
            cur_tok = NAME;
        }
    }

    return cur_tok;
}

double task4_6::solution::Expr(bool from_rec)
{
    double left = Term();

    while (true)
    {
        switch (cur_tok)
        {
        case PLUS:
            left += Term();
            break;
        case MINUS:
            left -= Term();
            break;
        default:
			if (!from_rec && cur_tok != END)
			{
				std::string err_str = "not correct expression at %1% line";
				throw err_str;
			}
            return left;
        }
    }
}

double task4_6::solution::Term()
{
    double left = Prim();

    while (true)
    {
        switch(cur_tok)
        {
        case MUL:
            left *= Prim();
            break;
        case DIV:
            if (double d = Prim())
            {
                left /= d;
                break;
            }
			else
			{
				std::string err_str = "zero div (%1%)";
				throw err_str;
			}
        default:
            return left;
        }
    }
}

double task4_6::solution::Prim()
{
    GetToken();

    switch (cur_tok)
    {
    case NUMBER:
        {
            double v = number_val;
            GetToken();
            return v;
        }
    case NAME:
        {
            std::string cur_name = name_val;
            auto it = names.find(cur_name);
			GetToken();
            if (it == names.end())
            {
				if (cur_tok == ASSIGN)
                {
                    double v2 = Expr(true);
                    names[cur_name] = v2 > 0 ? v2 + 0.5 : v2 - 0.5;
                    return names[cur_name];
                }
                else
                {
					std::string err_str = (boost::format("'%1%' variable not defined at line %2%" ) % cur_name % "%1%").str();
                    throw err_str;
                }
            }
			else if (cur_tok == ASSIGN)
			{
				std::string err_str = (boost::format("such variable '%1%' already exists (%2%)") % cur_name % "%1%").str();
                throw err_str;
			}
            
            return it->second;
        }
    case MINUS:
        return -Prim();
    case LP:
        {
            double e = Expr(true);
            if (cur_tok != RP)
			{
				std::string err_str = "not correct expression at %1% line";
				throw err_str;
			}
            GetToken();
            return e;
        }
    default:
		std::string err_str = "not correct expression at %1% line";
		throw err_str;
        return 0;
    }
}
