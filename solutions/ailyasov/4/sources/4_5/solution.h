#ifndef _TASK4_5_SOLUTION_H_
#define _TASK4_5_SOLUTION_H_

#include <vector>

#include <boost/thread/mutex.hpp>

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
            int max_;
            int min_;
            std::vector<int> max_values_;
            std::vector<int> min_values_;
            data_type const& data_;
            boost::mutex values_protector_;
            void add_min_max(data_type::const_iterator it);
    };

}

#endif // _TASK4_5_SOLUTION_H_

