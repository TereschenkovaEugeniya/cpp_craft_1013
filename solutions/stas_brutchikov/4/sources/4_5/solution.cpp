#include "solution.h"

#include <stdexcept>
#include <boost/thread.hpp>

task4_5::solution::solution( const data_type& data )
{
    data_  = data;
}


template<typename _Pr>
void extremum_in_vector(const std::vector<int>& in, int& extrem)
{
    _Pr pred;
    extrem = in.size() ? in[0] : 0;
    for(auto j = in.cbegin(); j != in.cend(); ++j)
    {
        if (!pred(extrem, *j))        
            extrem = *j;
    }
}

template<typename _Pr>
int extremum_in_data(const task4_5::data_type& in)
{
    boost::thread_group threads;
    std::vector<int> values(in.size());
    	    
    for(auto i = 0; i != in.size(); ++i)
        threads.create_thread(boost::bind(extremum_in_vector<_Pr>, boost::ref(in[i]), boost::ref(values[i])) );
    threads.join_all();

    int res = 0;
    threads.create_thread(boost::bind(extremum_in_vector<_Pr>, boost::ref(values),  boost::ref(res)) );
    threads.join_all();
	return res;
}

int task4_5::solution::get_min() const
{
    return extremum_in_data<std::less<int> >(data_);
}
int task4_5::solution::get_max() const
{
    return extremum_in_data<std::greater<int> >(data_);
}
