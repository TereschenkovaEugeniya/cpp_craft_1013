#include "solution.h"

#include <vector>
#include <algorithm>

#include <boost/thread.hpp>

static boost::mutex mtxMax;
static boost::mutex mtxMin;
static boost::mutex mtxNumberData;

task4_5::solution::solution( const data_type& data )
	:mMin(0), mMax(0), numberData(0)
{
	if (data.size())
	{
		mMax = *(*data.begin()).begin();
		mMin = *(*data.begin()).begin();
	} else
	{
		return;
	}

	boost::thread_group groupFindMinMAx;
	size_t threadsCount = boost::thread::hardware_concurrency(); 
	for( size_t i = 0; i < threadsCount; ++i )
	{
		groupFindMinMAx.create_thread( boost::bind( &solution::thread, this, boost::ref(data)));
	}
	groupFindMinMAx.join_all();
}

int task4_5::solution::get_min() const
{
	return mMin;
}
int task4_5::solution::get_max() const
{
	return mMax;
}

void task4_5::solution::thread(const data_type &data)
{
	size_t count = data.size();
	for(;;)
	{
		std::vector < int > temp;
		{
			boost::mutex::scoped_lock lock( mtxNumberData );
			if (numberData >= count)
				return;
			temp = *(data.begin() + numberData);
			++numberData;
		}
		findMinMax(temp);
	}
}

void task4_5::solution::findMinMax(const std::vector < int > &data)
{	
	for(std::vector < int >::const_iterator it = data.begin(); it!= data.end(); ++it)
	{
		{
			boost::mutex::scoped_lock lock( mtxMax );
			if (*it > mMax)
				mMax = *it;
		}
		{
			boost::mutex::scoped_lock lock( mtxMin );
			if (*it < mMin)
				mMin = *it;
		}
	}
}