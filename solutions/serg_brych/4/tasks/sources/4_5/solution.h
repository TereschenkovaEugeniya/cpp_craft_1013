#ifndef _TASK4_5_SOLUTION_H_
#define _TASK4_5_SOLUTION_H_

#include <vector>
#include <queue>
#include <boost/thread.hpp>
#include <boost/bind.hpp>



namespace task4_5
{
	typedef std::vector< std::vector< int > > data_type;
	
	class solution
	{
	public:
		explicit solution( const data_type& data );
		int get_min() const;
		int get_max() const;
	private:
		void calculate_result(const data_type& data);
		void process_thread(const data_type& data ,const int init_value);
		int m_min;
		int m_max;

		size_t m_current_vector;
		boost::thread_group m_threads;

		boost::mutex m_get_data_mutex;
		boost::mutex m_set_data_mutex;
		
	};
}

#endif // _TASK4_5_SOLUTION_H_