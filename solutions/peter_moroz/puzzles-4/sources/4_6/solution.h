#ifndef _TASK4_6_SOLUTION_H_
#define _TASK4_6_SOLUTION_H_

#include <map>
#include <vector>
#include <string>
#include <boost/noncopyable.hpp>

namespace task4_6
{
	typedef std::vector< std::string > lines;

	class MultilineCalculator : public boost::noncopyable {

	public:
	  MultilineCalculator();
	  ~MultilineCalculator();

	  double CalculateExpression(const std::string& expr);
	  double GetValueOf(const std::string& variable_name) const;

	private:
	  double GetValueOfExpression(const std::string& s, size_t& p);

	  double Expr(const std::string& s, size_t& p);
	  double Factor(const std::string& s, size_t& p);
	  double Number(const std::string& s, size_t& p);
	  double Term(const std::string& s, size_t& p);

	  std::string Variable(const std::string& s, size_t& p);
	  double Identifier(const std::string& s, size_t& p);

	private:
	  std::map<std::string, double> names_table_;
	};

	class solution
	{
	public:
		explicit solution( const lines& calulator_lines );
		int result_for( const std::string& key ) const;
	private:
		MultilineCalculator calculator_;
	};
}

#endif // _TASK4_5_SOLUTION_H_