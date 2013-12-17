#ifndef _TASK4_5_SOLUTION_H_
#define _TASK4_5_SOLUTION_H_

#include <vector>
#include <iterator>
#include <boost/thread.hpp>


namespace task4_5
{
	typedef std::vector< std::vector< int > > data_type;

    template < typename T >
    class magic_iterator : public std::iterator< std::input_iterator_tag, const T >
    {
        typedef std::vector< std::vector< T > > data_type;

        const data_type&                   data_;
        typename data_type::const_iterator it_;
        size_t                             index_;
        bool                               is_end_;

    public:
        magic_iterator( const data_type& data, size_t index ) :
            data_( data ),
            index_( 0 ),
            is_end_( false )
        {
            it_ = data_.cbegin();
            *this += index; 
        }

        bool is_end()
        {
            return is_end_;
        }

        reference operator *()
        {
            return ( *it_ )[index_];
        }

        pointer operator ->()
        {
            return &( *it_ )[index_];
        }

        void operator +=( size_t x )
        {
            if( is_end_ )
            {
                return;
            }
            index_ += x;
            while( index_ >= it_->size() )
            {
                index_ = index_ - it_->size();
                ++it_;
                if( it_ == data_.end() )
                {
                    is_end_ = true;
                    return;
                }

            }
        }

        value_type operator ++( int )
        {
            *this += 1;
            return *this;
        }

        value_type operator ++()
        {          
            data_iterator x = *this;
            *this += 1;
            return x;
        }

        bool operator ==( const magic_iterator& it )
        {
            return  ( is_end_ && it.is_end_ ) ||
                ( data_ == it.data_ && it_ == it.it_ && index_ == it.index_ );
        }

        bool operator !=( const magic_iterator& it )
        {
            return ! ( *this == it );
        }

    };

	class solution
	{
        const data_type& data_;
        int min_;
        int max_;
        bool need_calculate_;
        boost::mutex mtx_;

        void calc_proc( magic_iterator<int>& it, size_t step );
        void calculate();
	public:
		explicit solution( const data_type& data );
		int get_min() const;
		int get_max() const;
	};



}

#endif // _TASK4_5_SOLUTION_H_
