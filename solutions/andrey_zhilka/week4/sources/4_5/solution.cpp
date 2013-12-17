#include "solution.h"
#include <iterator>
#include <utility>

task4_5::solution::solution( const data_type& data )
{
	boost::mutex::scoped_lock wait_process( this->minmax_process_ );
	
	min_ = INT_MAX;
	max_ = INT_MIN;

	boost::thread_group processes;
	
	if ( data.empty() )
	{
		min_ = 0;
		max_ = 0;
	}
	else 
	{
		for ( size_t i = 0; i < data.size(); i++ )
		{
			processes.create_thread( boost::bind( &task4_5::solution::minmax, this, data[i] ) );
		}

		processes.join_all();
	}
}

int task4_5::solution::get_min() const
{
	boost::mutex::scoped_lock wait_min( minmax_process_ );
	return this->min_;
}
int task4_5::solution::get_max() const
{
	boost::mutex::scoped_lock wait_max( this->minmax_process_ );
	return this->max_;
}

void task4_5::solution::minmax(std::vector<int>& data)
{	
	std::pair< data_iterator, data_iterator > result =
		boost::minmax_element( data.begin(), data.end() );

	{
		boost::mutex::scoped_lock wait_min( this->mutex_for_min_ );
		if ( min_ > *result.first )
		{
			min_ = *result.first;
		}
	}

	{
		boost::mutex::scoped_lock wait_max( this->mutex_for_max_ );
		if ( max_ < *result.second )
		{
			max_ = *result.second;
		}
	}
}