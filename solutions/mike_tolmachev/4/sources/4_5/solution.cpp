#include "solution.h"

task4_5::solution::solution( const data_type& data ) : min_(0), max_(0)
{		
	if (!data.empty())
	{
		data_ = data;

		min_ = std::numeric_limits< int >().max();
		max_ = std::numeric_limits< int >().min();

		cur_it_ = data_.begin();

		boost::thread_group thread_group;
		for(int i = 0; i < 4; ++i)
		{
			thread_group.create_thread(boost::bind(&solution::thread_fun, this));
		}

		thread_group.join_all();
	}
}

int task4_5::solution::get_min() const
{
	return min_;
}
int task4_5::solution::get_max() const
{
	return max_;
}
task4_5::data_type::const_iterator task4_5::solution::GetVector()
{
	boost::mutex::scoped_lock lock(mtx_vec_);

	auto prev_it = cur_it_;
	if (cur_it_ != data_.end())
	{
		++cur_it_;
	}

	return prev_it;
}
void task4_5::solution::thread_fun()
{
    auto it = GetVector();

	while(it != data_.end())
	{
		auto minmax = std::minmax_element(it->begin(), it->end());
		boost::mutex::scoped_lock lock(mtx_minmax_);
		if (min_ > *minmax.first)
		{
			min_ = *minmax.first;
		}
		if (max_ < *minmax.second)
		{
			max_ = *minmax.second;
		}

		it = GetVector();
	}
}