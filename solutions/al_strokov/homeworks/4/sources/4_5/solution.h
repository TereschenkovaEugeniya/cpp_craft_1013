#ifndef _TASK4_5_SOLUTION_H_
#define _TASK4_5_SOLUTION_H_

#include "VectorIntMinMax.h"

#include <vector>
#include <iostream>
#include <boost/thread/thread.hpp>

namespace task4_5
{
	typedef std::vector< std::vector< int > > data_type;

	class solution
	{
	private:
		data_type data_;
		int min_;
		int max_;
	public:
		explicit solution( const data_type& data );
		int get_min() const;
		int get_max() const;
		void render();
	};
}

#endif // _TASK4_5_SOLUTION_H_

