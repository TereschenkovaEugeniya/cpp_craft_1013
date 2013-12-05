#ifndef _TASK4_5_SOLUTION_H_
#define _TASK4_5_SOLUTION_H_

#include <vector>
#include <math.h>
#include <boost\thread.hpp>
#include <math.h>
#include <limits>

namespace task4_5
{
	typedef std::vector< std::vector< int > > data_type;

	class solution
	{
	private:
		std::vector< std::vector< int > > a;
		mutable boost :: mutex mtx;
		mutable int MIN;
		mutable int MAX;
	public:
		explicit solution( const data_type& data );
		int get_min() const;
		int get_max() const;
		void count_min(std::vector< int >&) const;
		void count_max(std::vector< int >&) const;
		void flows_to_find_min() const;
		void flows_to_find_max() const;
	};
}

#endif // _TASK4_5_SOLUTION_H_