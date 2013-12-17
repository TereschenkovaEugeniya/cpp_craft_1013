#ifndef _TASK4_5_SOLUTION_H_
#define _TASK4_5_SOLUTION_H_

#include <vector>
#include <boost/thread.hpp>
#include <boost/algorithm/minmax.hpp>
#include <boost/algorithm/minmax_element.hpp>

namespace task4_5
{
	typedef std::vector< std::vector< int > > data_type;
	typedef std::vector< int >::iterator data_iterator;

	class solution
	{
	private :
		int min_;
		int max_;
		mutable boost::mutex mutex_for_min_;
		mutable boost::mutex mutex_for_max_;

		mutable boost::mutex minmax_process_;

		void minmax( std::vector< int >& );

	public :
		explicit solution( const data_type& data );
		int get_min() const;
		int get_max() const;
	};
}

#endif // _TASK4_5_SOLUTION_H_