#include "solution.h"


task4_5::solution::solution( const data_type& data, const size_t NumThreads )
	: Max(0), Min(0)
{
	if(data.size())
		FindMinMax(data, NumThreads);
}

int task4_5::solution::get_min() const
{
	return Min;
}
int task4_5::solution::get_max() const
{
	return Max;
}

void task4_5::solution::FindMinMax(const data_type& Dat, const size_t NumThreads)
{
	Max = std::numeric_limits<int>::min();
	Min = std::numeric_limits<int>::max();

	size_t num_vectors = static_cast<size_t>(ceil(static_cast<double>(Dat.size()/(static_cast<double>(NumThreads)))));

	for (size_t i = 0; i < NumThreads; ++i)
	{
		size_t pos_start, pos_end;

		//iterators std::vector< std::vector< int > > range for each thread
		pos_start = i*num_vectors;
		pos_end = (i+1)*num_vectors;

		if(pos_start >= Dat.size())
		{
			pos_start = 0;
			pos_end = 0;
		}
		else if (pos_end > Dat.size())
				pos_end = Dat.size();

		tg.create_thread(boost::bind(&task4_5::solution::ThreadedSearch, this, Dat, pos_start, pos_end));
	}

	tg.join_all();



}

void  task4_5::solution::ThreadedSearch(const data_type& Dat, size_t shift_Start, size_t shift_End)
{

	data_type::const_iterator it;
	for(it = Dat.begin() + shift_Start; it != Dat.begin() + shift_End; ++it)
	{
		std::pair< std::vector< int >::const_iterator, std::vector< int >::const_iterator > result = boost::minmax_element(it->begin(), it->end());

		{
			boost::mutex::scoped_lock protect_min_max_update(Mtx_MinMax_update);
			if(*result.first < Min) Min = *result.first;
			if(*result.second > Max) Max = *result.second;
		}
	}

}
