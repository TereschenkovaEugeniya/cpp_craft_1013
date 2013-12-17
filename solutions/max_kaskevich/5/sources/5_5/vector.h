#ifndef _TASK5_5_VECTOR_H_
#define _TASK5_5_VECTOR_H_

#include <cstdlib>
#include <memory>
#include <sstream>

namespace task5_5
{
	template< typename T >
	class vector
	{
        static const size_t reserved_size_begin = 4;
        static const size_t reserved_size_rate = 2;
        size_t reserved_size_;
        size_t size_;
        std::unique_ptr< T[] > data_;

        void check_reserve();
        void check_index( size_t index ) const;
	public:
		typedef T* iterator ;
		typedef const T* const_iterator;
	public:
		explicit vector();
		vector( const vector& copy );
		vector& operator=( const vector& copy_from );

		void push_back( const T& value );
		void insert( const size_t index, const T& value );

		T& operator[]( const size_t index );
		const T& operator[]( const size_t index ) const;

		void resize( const size_t amount );
		void reserve( const size_t amount );

		size_t size() const;
		size_t capacity() const;
		bool empty() const;

		iterator begin();
		iterator end();

		const_iterator begin() const;
		const_iterator end() const;
	};

	template< typename T >
	vector< T >::vector() :
        reserved_size_( reserved_size_begin ),
        size_( 0 ),
        data_( new T[ reserved_size_ ] )
	{
	}

	template< typename T >
	vector< T >::vector( const vector< T >&  vec)
	{
        *this = vec;
	}

	template< typename T >
	vector< T >& vector< T >::operator=( const vector< T >&  vec)
	{
        reserved_size_ = vec.reserved_size_;
        size_ = vec.size_;
        data_.reset( new T[ reserved_size_ ] );
        std::copy( vec.begin(), vec.end(), begin() );
		return *this;
	}

	template< typename T >
	void vector< T >::push_back( const T& t )
	{
        check_reserve();
        data_[ size_ ] = t;
        ++size_;
	}

	template< typename T >
	void vector< T >::insert( const size_t index, const T& t )
	{
        check_index( index );
        check_reserve();
        for( size_t i = size_; i > index; --i )
        {
            data_[ i ] = data_ [ i - 1 ];
        }
        data_[ index ] = t;
        ++size_;
	}

	template< typename T >
	T& vector< T >::operator[]( const size_t index )
	{
        check_index( index );
        return data_[ index ];
	}

	template< typename T >
	const T& vector< T >::operator[]( const size_t index ) const
	{
        check_index( index );
        return data_[ index ];
	}

	template< typename T >
	void vector< T >::resize( const size_t new_size )
	{
        if( new_size > reserved_size_)
        {
            reserve( new_size );
        }
        if( new_size > size_ )
        {
            for( size_t i = size_; i < new_size; ++i )
            {
                data_[ i ] = T();
            }
        }
        size_ = new_size;
	}

	template< typename T >
	void vector< T >::reserve( const size_t new_reserve_size )
	{
        if ( new_reserve_size > reserved_size_ )
        {
            reserved_size_ = new_reserve_size;
            T* new_data = new T[ reserved_size_ ];
            std::copy( begin(), end(), new_data );
            data_.reset( new_data );
        }
	}

	template< typename T >
	size_t vector< T >::size() const
	{
		return size_;
	}
	template< typename T >
	size_t vector< T >::capacity() const
	{
		return reserved_size_;
	}
	template< typename T >
	bool vector< T >::empty() const
	{
		return !size_;
	}
	template< typename T >
	typename vector< T >::iterator vector< T >::begin()
	{
		return data_.get();
	}
	template< typename T >
	typename vector< T >::iterator vector< T >::end()
	{
		return data_.get() + size_;
	}
	
	template< typename T >
	typename vector< T >::const_iterator vector< T >::begin() const
	{
		return data_.get();
	}
	template< typename T >
	typename vector< T >::const_iterator vector< T >::end() const
	{
		return data_.get() + size_;
	}

    template< typename T >
    void vector< T >::check_reserve()
    {
        if( size_ == reserved_size_ )
        {
            reserve( reserved_size_ * reserved_size_rate );
        }
    }

    template< typename T >
    void vector< T >::check_index( size_t index ) const
    {
        if ( index >= size_ )
        {
            std::ostringstream msg;
            msg << index << " position is out of " << size_ << " size vector";
            throw std::out_of_range( msg.str() );
        }
    }
}

#endif // _TASK5_5_VECTOR_H_

