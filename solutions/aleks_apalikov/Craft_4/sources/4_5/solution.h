#ifndef _TASK4_5_SOLUTION_H_
#define _TASK4_5_SOLUTION_H_

#include <vector>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

namespace task4_5
{
	typedef std::vector< std::vector< int > > data_type;
	typedef std::vector< int > data_vector;
	class Min_routine
	{
	private:
		int& val;
		data_vector& arr;
	public:
		Min_routine(int& v, data_vector& a):val(v),arr(a)
		{
		}
		int get()
		{
			return val;
		}
		void operator()()
		{
			for(data_vector::iterator it = arr.begin(); it != arr.end(); ++it)
			{
				if(*it < val)
				{
					val = *it;
				}
			}
		}
	};
	class Max_routine
	{
	private:
		int& val;
		data_vector& arr;
	public:
		Max_routine(int& v, data_vector& a):val(v),arr(a)
		{
		}
		int get()
		{
			return val;
		}
		void operator()()
		{
			for(data_vector::iterator it = arr.begin(); it != arr.end(); ++it)
			{
				if(*it > val)
				{
					val = *it;
				}
			}
		}
	};

	class solution
	{
	private:
		data_type data_;
		data_vector results;
		data_vector resultsMax;
		boost::thread_group threads_;
		size_t thread_amount;
		int minimum;
		int maximum;
	public:
		explicit solution( const data_type& data );
		int get_min() const;
		int get_max() const;
	};
}

#endif // _TASK4_5_SOLUTION_H_
