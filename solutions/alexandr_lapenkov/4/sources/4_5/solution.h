#ifndef _TASK4_5_SOLUTION_H_
#define _TASK4_5_SOLUTION_H_

#include <vector>
#include <limits>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>

namespace task4_5
{
	typedef std::vector< std::vector< int > > data_type;

	class solution
	{
	private:
		data_type data_;
		mutable boost::mutex result_protector;
		mutable int result; 

		void solve(const int& (*comp)(const int &, const int& )) const;
		void process_row(std::vector< data_type::value_type::value_type >& arr,
						const int& (*comp)(const int &, const int& ) ) const;

	public:
		explicit solution( const data_type& data );
		int get_min() const;
		int get_max() const;
	};
}

#endif // _TASK4_5_SOLUTION_H_