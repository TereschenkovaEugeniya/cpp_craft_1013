#ifndef _TASK4_5_SOLUTION_H_
#define _TASK4_5_SOLUTION_H_

#include <vector>
#include <limits>
#include <math.h>
#include <boost\noncopyable.hpp>
#include <boost\thread.hpp>
#include <boost\algorithm/minmax_element.hpp>


const static size_t NUM_THREADS = 2;

namespace task4_5
{
	typedef std::vector< std::vector< int > > data_type;

	class solution: virtual protected boost::noncopyable
	{
	public:
		explicit solution( const data_type& data, const size_t NumThreads = NUM_THREADS);
		int get_min() const;
		int get_max() const;
	private:
		int Max, Min;
		boost::thread_group tg;
		boost::mutex Mtx_MinMax_update;

		void FindMinMax(const data_type& Dat, const size_t NumThreads);
		void ThreadedSearch(const data_type& Dat, size_t shift_Start, size_t shift_End);


	};
}

#endif // _TASK4_5_SOLUTION_H_

