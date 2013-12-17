#include "solution.h"

task4_5::solution::solution( const data_type& data )
{
	data_ = data;
}

void task4_5::solution::solve(const int& (*comp)(const int &, const int& )) const
{
	if(data_.size() == 0)
	{
		result = 0;
		return ;
	}

	boost::thread_group t; 
	for(size_t i = 0; i < data_.size(); ++i )
	{
		t.create_thread( boost::bind(&task4_5::solution::process_row, this, data_[i], comp) );
	}

	t.join_all();
}

void task4_5::solution::process_row(std::vector< data_type::value_type::value_type >& arr,
									const int& (*comp)(const int &, const int& )) const
{
	int local;
	bool f = 0;

	BOOST_FOREACH( int var, arr )
	{
		if(!f)local = var, f = 1;
		else  local = comp(local, var);
	}

	{
		boost::mutex::scoped_lock lock( result_protector );
		result = comp(result, local);	
	}
}

int task4_5::solution::get_min() const
{
	result = std::numeric_limits< int >().max();
	solve( &std::min< int > );
	return result;
}
int task4_5::solution::get_max() const
{
	result = std::numeric_limits< int >().min();
	solve( &std::max< int > );
	return result;
}
