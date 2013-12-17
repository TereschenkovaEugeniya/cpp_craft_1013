#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include "solution.h"
#include <boost/lexical_cast.hpp>

/******************************************************************** 
  The syntax of arithmetical expressions is declared as set of rules. 
  Each of them is processed by method with appropriate name.

  <Expr> ::= <Term> { <Operator1> <Term> }
  <Term> ::= <Factor> { <Operator2> <Factor> }
  <Factor> ::= <Factor> | <Identifier> | <Number> | '(' <Expr> ')'
  <Identifier> ::= <Letter> { <Letter> | <Digit> }
  <Number> ::= <Digit> { <Digit> }
  <Digit> ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
  <Letter> ::= 'A' | ... | 'Z' | 'a' | ... | 'z'
 *******************************************************************/

namespace task4_6
{

MultilineCalculator::MultilineCalculator() {}
MultilineCalculator::~MultilineCalculator() {}

double MultilineCalculator::CalculateExpression(const std::string& expr) {
  std::string s(expr);

  s = std::string(s.begin(), std::remove_if(s.begin(), s.end(), &isspace));

  size_t p = 0;
  // extract declaration of variable
  std::string name = Variable(s, p);
  // skip assignment operator
  ++p;
  // calculate value of expression
  double value = GetValueOfExpression(s, p);
  names_table_[name] = value;
  return value;
}
double MultilineCalculator::GetValueOf(const std::string& variable_name) const {
  std::map<std::string, double>::const_iterator it;
  it = names_table_.find(variable_name);
  if (it == names_table_.end()) {
    std::stringstream msg;
    msg << "There are no variable with name: " << variable_name;
    throw std::logic_error(msg.str());
  }
  return it->second;
}

double MultilineCalculator::GetValueOfExpression(const std::string& s, size_t& p) {
  double result = Expr(s, p);

  if (p < s.length()) {
    std::stringstream msg;
    msg << "GetValueOfExpression - Invalid character in position: " << p 
		<< " s.length() = " << s.length() << std::endl;
	  throw std::logic_error(msg.str());
  }
  return result;
}
double MultilineCalculator::Expr(const std::string& s, size_t& p) {
  char op = ' ';
  double result = Term(s, p);

  while (p < s.length() && (s[p] == '+' || s[p] == '-')) {
    op = s[p];
    ++p;
    if (op == '+')
      result += Term(s, p);
    else if (op == '-')
      result -= Term(s, p);
  } // while (p < s.length() && (s[p] == '+' || s[p] == '-'))
  return result;
}
double MultilineCalculator::Factor(const std::string& s, size_t& p) {
  if (p > s.length()) 
    throw std::logic_error("Factor - Unexpected end of string");

  if (isdigit(s[p]))
    return Number(s, p);
  if (isalpha(s[p]))
    return Identifier(s, p);

  double result = 0;
  std::stringstream msg;

  if (s[p] == '(') {
    ++p;
    result = Expr(s, p);

    if (p >= s.length() || s[p] != ')') {
      msg << "The \')\' is expected in position: " << p;
      throw std::logic_error(msg.str());
    }
  	++p;
  } else {
    msg << "Invalid character \'" << s[p] 
      << "\' in position: " << p;
    throw std::logic_error(msg.str());
  }
  return result;
}
double MultilineCalculator::Number(const std::string& s, size_t& p) {
  size_t start_pos = p;
  std::stringstream msg;
  if (p < s.length() && (s[p] == '+' || s[p] == '-')) ++p;
  if (p >= s.length() || !isdigit(s[p])) {
    msg << "The number is expected in position: " << p;
    throw std::logic_error(msg.str());
  }

  do {
    ++p;
  } while (p < s.length() && isdigit(s[p]));

  /* it will be better to get separator from locale,
    or make it adjustable via public setter */
  if (p < s.length() && s[p] == '.') {
	  ++p;
	  if (p >= s.length() || !isdigit(s[p])) {
		  msg << "The number is expected in position: " << p;
		  throw std::logic_error(msg.str());
	  }
	  while (p < s.length() && isdigit(s[p])) ++p;
  }

  double result = 0.0;
  std::string number_text = s.substr(start_pos, p - start_pos);
  try {
	  result = boost::lexical_cast<double>(number_text.c_str());
  } catch (boost::bad_lexical_cast&) {
	  msg << "Invalid format of number " << number_text;
	  throw std::logic_error(msg.str());
  }
  return result;
}
double MultilineCalculator::Term(const std::string& s, size_t& p) {
  char op = ' ';
  double result = Factor(s, p);
  while (p < s.length() && (s[p] == '*' || s[p] == '/')) {
    op = s[p];
    ++p;
    if (op == '*') {
      result *= Factor(s, p);
    } else if (op == '/') {
      /* variable pos is needed to propagate 
       information only if exception occured */
      size_t pos = p + 1;
      double divisor = Factor(s, p);
      if (divisor == 0) {
        std::stringstream msg;
        msg << "Term - Division by zero in position: " << pos;
        throw std::logic_error(msg.str());
      }
      result /= divisor;
    }
  } // while (p < s.length() && (s[p] == '*' || s[p] == '/'))
  return result;
}
std::string MultilineCalculator::Variable(const std::string& s, size_t& p) {
  size_t start_pos = p;
  std::stringstream msg;
  if (p >= s.length() || !isalpha(s[p]))
    throw std::logic_error("expression must begin from destination variable.");
  while (isalnum(s[p])) ++p;
  
  std::string variable = s.substr(start_pos, p - start_pos);
  if (names_table_.find(variable) != names_table_.end()) {
    msg << "Variable with name " << variable << " already defined ";
    throw std::logic_error(msg.str());
  }

  names_table_.insert(make_pair(variable, 0.0));
  //names_table_.insert(std::pair<std::string, double>(variable, 0));
  return variable;
}
double MultilineCalculator::Identifier(const std::string& s, size_t& p) {
  size_t start_pos = p;
  std::stringstream msg;
  if (p >= s.length() || !isalpha(s[p])) {
    msg << "The identifier is expected in position: " << p;
    throw std::logic_error(msg.str());
  }

  do {
    ++p;
  } while (p < s.length() && isalnum(s[p]));

  std::string name = s.substr(start_pos, p - start_pos);
  std::map<std::string, double>::iterator it = names_table_.find(name);
  if (it == names_table_.end()) {
    msg << "Unknown identifier in position: " << p;
    throw std::logic_error(msg.str());
  }
  return it->second;
}

}

task4_6::solution::solution( const lines& calulator_lines )
{
	for (size_t i = 0; i < calulator_lines.size(); ++i) {
		try {
		  calculator_.CalculateExpression(calulator_lines[i]);
		} catch (std::logic_error& e) {
			std::stringstream msg;
			msg << e.what() << "(line: " << i << ')';
			throw std::logic_error(msg.str());
		}
	}
}

int task4_6::solution::result_for( const std::string& key ) const
{
	int result = 0;
	try {
		double value = calculator_.GetValueOf(key);
		double x = 0.0;
		result = (modf(value, &x) >= 0.5)
  			   ? static_cast<int>(x + 1.0)
	  		   : static_cast<int>(x);
	} catch (std::logic_error& e) {
		std::cerr << e.what() << std::endl;
	}
	return result;
}
