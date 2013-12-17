#include "solution.h"

task4_6::solution::solution( const lines& calulator_lines )
{
	size_t finded_char = std::string::npos;
	std::string polish_str, value_name;
	for(int i = 0; i < calulator_lines.size(); i++)
	{
		finded_char = calulator_lines[i].find('=');
		if(finded_char!= std::string::npos)
		{
			try
			{
				polish_str = get_pl_revers(calulator_lines[i].substr(finded_char + 1));
				value_name = calulator_lines[i].substr(0, finded_char);
			}
			catch(const std::logic_error &er)
			{
				throw std::logic_error( (boost::format(er.what()) % i).str());
			}
			value_name.erase(remove_if(value_name.begin(), value_name.end(), std::isspace));
			if(vars.find(value_name) != vars.end())
			{
				throw std::logic_error( (boost::format("such variable '%1%' already exists %2%") % value_name % i).str() ); 
			}
			vars[value_name] = calc_pl_expr(polish_str);
		}
		else
		{
			throw std::logic_error((boost::format("not correct expression at %1% line")% i).str()); 
		}
	}
	int h = 1;
}

int task4_6::solution::result_for( const std::string& key ) const
{
	return static_cast<int>( vars.at( key ) + ( vars.at( key ) >= 0 ? .5 : -.5) );
}


const std::string task4_6::solution::get_pl_revers(const std::string& expression)
{
	std::stack<char> op_stack;
	std::stringstream revers_polish;
	lines result;
	
	char current_symbol;
	for(int i = 0; i < expression.size(); i++)
	{
		current_symbol = expression[i];
		if(is_number(current_symbol)||std::isalpha(static_cast<int>(current_symbol)))
		{
			revers_polish << current_symbol; 
		}
		else
		{
			if(current_symbol == '(')
			{
				op_stack.push(current_symbol);
			}
			else if(is_operation(static_cast<int>(current_symbol)))
			{
				revers_polish << " ";
				if(!op_stack.empty())
				{
					if( operation_priority(op_stack.top()) > operation_priority(current_symbol))
					{
						revers_polish << op_stack.top() << " ";
						op_stack.pop();
					}
				}
				op_stack.push(current_symbol);

			}
			else if(current_symbol == ')')
			{
				while(op_stack.top() != '(')
				{
					revers_polish << " " << op_stack.top();
					op_stack.pop();
					if(op_stack.empty())
					{
						throw std::logic_error("not correct expression at %1% line"); 
					}
				}
				op_stack.pop();
			}
		}
	}
	revers_polish << " ";
	while(!op_stack.empty())
	{
		revers_polish << op_stack.top() << " ";
		op_stack.pop();
	}
	return revers_polish.str();
}

double task4_6::solution::calc_pl_expr(const std::string& polish_expression)
{
	double result = 0.0;
	std::string value;
	double value1, value2;

	std::stringstream ss_in(polish_expression);
	std::stack<double> values_stack;

	while(ss_in >> value)
	{
		if(is_operation(value[0]))
		{
			if(values_stack.empty())
				throw std::logic_error("not correct expression at %1% line"); 
			value2 =  values_stack.top();
			values_stack.pop();
			if(values_stack.empty())
				throw std::logic_error("not correct expression at %1% line"); 
			value1 =  values_stack.top();
			values_stack.pop();

			switch (value[0])
			{
			case '+':
				result = value1 + value2;
				break;
			case '-':
				result = value1 - value2;
				break;
			case '*':
				result = value1 * value2;
				break;
			case '/':
				if(value2 == 0.0)
				{
					throw std::logic_error("zero div (%1%)");
				}
				result = value1 / value2;
				break;
			default:
				break;
			}
			values_stack.push(result);
		}
		else
		{
			if(is_number(value))
			{
				values_stack.push(boost::lexical_cast<double>(value));
			}
			else
			{
				if(vars.find(value) != vars.end())
				{
					values_stack.push(vars[value]);
				}
				else
				{
					
					throw std::logic_error((boost::format("'%1%' variable not defined at line %2%") % value % "%1%").str()); 
				}
			}
		}
	}

	if(!values_stack.empty())
	{
		result = values_stack.top();
		values_stack.pop();
	}

	return result;
}

bool task4_6::solution::is_number(char ch)
{
	return (std::isdigit(ch) || ch == '.');
}

bool task4_6::solution::is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && (std::isdigit(*it) || *it == '.')) ++it;
    return !s.empty() && it == s.end();
}

bool task4_6::solution::is_operation(char ch)
{
	if((ch == '*')||(ch == '+')||(ch == '-')||(ch == '/'))
		return true;
	else
		return false;
}

uint32_t task4_6::solution::operation_priority(char ch)
{
	if((ch == '+')||(ch == '-'))
	{
		return 1;
	}else if((ch == '/')||(ch == '*'))
	{
		return 2;
	}
	return 0;
}
