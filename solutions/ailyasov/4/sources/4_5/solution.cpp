#include <algorithm>
#include <vector>
#include <limits>

#include <boost/thread/thread.hpp>
#include <boost/cstdint.hpp>
#include <boost/thread/mutex.hpp>

#include "solution.h" 

using boost::uint32_t;

void task4_5::solution::add_min_max(data_type::const_iterator it)
{
    std::vector<int> const& v = *it;
    if(!v.empty())
    {
        int min = *v.begin();
        int max = *v.begin(); 
        for(std::vector<int>::const_iterator itt = v.begin() + 1; itt != v.end(); ++itt)
        {
            if(*itt < min)
                min = *itt;
            if(*itt > max)
                max = *itt;
        }
        boost::mutex::scoped_lock lock ( values_protector_ );
        max_values_.push_back( max );
        min_values_.push_back( min );
    }
} 

task4_5::solution::solution( const data_type& data ) : max_(0), min_(0),data_(data) 
{
    max_values_.reserve(data_.size());
    min_values_.reserve(data_.size());

    boost::thread_group t_group;
    for(data_type::const_iterator it = data_.begin(); it != data_.end(); ++it)
        t_group.create_thread(boost::bind(&task4_5::solution::add_min_max,this,it));
    t_group.join_all();

    if(!min_values_.empty())
    {
        min_ = *std::min_element(min_values_.begin(), min_values_.end());
    }

    if(!max_values_.empty())
    {
        max_ = *std::max_element(max_values_.begin(), max_values_.end()); 
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
