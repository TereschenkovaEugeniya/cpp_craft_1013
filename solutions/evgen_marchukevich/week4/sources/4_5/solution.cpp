#include "solution.h"


task4_5::solution::solution( const data_type& data )
{
	a=data;
	MAX = std :: numeric_limits<int>().min();
	MIN = std :: numeric_limits<int>().max();
	flows_to_find_min();
	flows_to_find_max();
}

void task4_5::solution::count_min(std::vector< int > &b) const
{	
	int Min = std :: numeric_limits<int>().max();
	for(size_t i=0;i<b.size();i++)
		Min = std :: min(Min,b[i]);
	{
		boost :: mutex :: scoped_lock lock(mtx);
		MIN = std :: min(Min,MIN);
	}
}

void task4_5::solution::flows_to_find_min() const
{
	if (a.size()==0) 
		{
			MIN = 0;
			return;
		}
	boost :: thread_group group;
	for(size_t i=0;i<a.size();i++)
		group.create_thread(boost :: bind (&task4_5 :: solution :: count_min,this,a[i]));	
	group.join_all();	
}

void task4_5::solution::count_max(std::vector< int > &b) const
{	
	int Max = std :: numeric_limits<int>().min();
	for(size_t i=0;i<b.size();i++)
		Max = std :: max(Max,b[i]);
	{
		boost :: mutex :: scoped_lock lock(mtx);
		MAX = std :: max(MAX, Max);
	}
}

void task4_5::solution::flows_to_find_max() const
{
	if (a.size()==0) 
		{
			MAX = 0;
			return;
		}
	boost :: thread_group group;
	for(size_t i=0;i<a.size();i++)
		group.create_thread(boost :: bind (&task4_5 :: solution :: count_max,this,a[i]));	
	group.join_all();	
}


int task4_5::solution::get_min() const
{	
	return MIN;
}
int task4_5::solution::get_max() const
{	
	return MAX;
}
