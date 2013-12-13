#include "solution.h"
#include <limits>

task4_5::solution::solution( const data_type& data )
{
	thread_amount = data.size();
	this->data_ = data;
//	results.reserve(data_.size());
	results.assign(data_.size(), std::numeric_limits<int>::max());

//	resultsMax.reserve(data_.size());
	resultsMax.assign(data_.size(), std::numeric_limits<int>::min());

	for( size_t i = 0 ; i < thread_amount ; ++i )
	{
		Min_routine mini(results[i], data_[i]);
		threads_.create_thread(mini) ;
	}
	for( size_t i = 0 ; i < thread_amount ; ++i )
	{
		Max_routine maxi(resultsMax[i], data_[i]);
		threads_.create_thread(maxi) ;
	}
	threads_.join_all();
	minimum = std::numeric_limits<int>::max();
	for (std::vector<int>::iterator it = results.begin(); it < results.end(); ++it)
	{
		if(*it < minimum)
			minimum = *it;
	}
	maximum = std::numeric_limits<int>::min();
	for (std::vector<int>::iterator it = resultsMax.begin(); it < resultsMax.end(); ++it)
	{
		if(*it > maximum)
			maximum = *it;
	}
	if(data.size() == 0)
	{
		minimum = maximum = 0;
	}
}

int task4_5::solution::get_min() const
{
	return minimum;
}
int task4_5::solution::get_max() const
{
	return maximum;
}
