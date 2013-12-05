#ifndef _TASK5_5_VECTOR_H_
#define _TASK5_5_VECTOR_H_

#include <cstdlib>
#include <vector>

namespace task5_5
{

	template< typename T >
	class vector
	{
    public:
		typedef T* iterator ; // you could change this
		typedef const T* const_iterator; // you could change this
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
    private:
        static const size_t def_size = 0;
		static const size_t def_capacity = 4;
		static const size_t def_mul = 2;

        size_t capacity_;        
        size_t size_;
        std::unique_ptr< T[] > data_;
        
	    void check_index( const size_t& index );

	};

	// TODO, please realise the rest methods according to the tests

	template< typename T >
	vector< T >::vector() : capacity_(def_capacity), size_(def_size), data_(new T[capacity_])
	{
	}
	template< typename T >
    vector< T >::vector( const vector< T >&  copy ):capacity_ ( copy.capacity_ ), 
        size_ ( copy.size_ ), data_( new T[capacity_] )
    {
		std::copy( copy.begin(), copy.end(), this->begin() );        
	}

	template< typename T >
	vector< T >& vector< T >::operator=( const vector< T >&  copy_from)
	{
        if ( copy_from.capacity_ > this->capacity_)
        {
            this->data_.reset( new T[copy_from.capacity_] );
        }

        this->size_ = copy_from.size_;
		this->capacity_ = copy_from.capacity_;	

		std::copy( copy_from.begin(), copy_from.end(), this->begin() );

		return *this;
	}

	template< typename T >
	void vector< T >::push_back( const T& value)
	{
        if ( size_ == capacity_ )
		{
			reserve( def_mul * capacity_ );
		}
		data_[size_] = value;
		++size_;
	}

	template< typename T >
	void vector< T >::insert( const size_t index, const T& value )
	{
		check_index( index );
		if ( size_ == capacity_ )
		{
			reserve( def_mul * capacity_ );
		}

		for ( size_t i = size_; i > index; --i )
		{
			data_[i] = data_[i-1];
		}

		data_[index] = value;
		++size_;
	}

	template< typename T >
	T& vector< T >::operator[]( const size_t index )
	{
        check_index ( index );
		return data_[index];
	}

	template< typename T >
	const T& vector< T >::operator[]( const size_t index ) const
	{
        check_index( index );
		return data_[index];
	}

	template< typename T >
	void vector< T >::resize( const size_t amount )
	{
        reserve( amount );

        for ( size_t i = size_; i < amount; ++i )
		{
			data_[i] = T();
		}

        size_ = amount;
	}
	template< typename T >
	void vector< T >::reserve( const size_t  amount )
	{
        if ( amount > capacity_ )
        {            
            T* new_data = new T[ amount ];
            std::copy( begin(), end(), new_data );
            data_.reset( new_data );

            capacity_ = amount;
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
		return capacity_;
	}
	template< typename T >
	bool vector< T >::empty() const
	{
		return size_ == 0;
	}
	template< typename T >
	typename vector< T >::iterator vector< T >::begin()
	{
		return &data_[0];
	}
	template< typename T >
	typename vector< T >::iterator vector< T >::end()
	{
		return &data_[size_];
	}
	
	template< typename T >
	typename vector< T >::const_iterator vector< T >::begin() const
	{
		return &data_[0];
	}
	template< typename T >
	typename vector< T >::const_iterator vector< T >::end() const
	{
		return &data_[size_];
	}

    template< typename T >
	typename void vector< T >::check_index( const size_t& index )
	{
		if ( index >= size_ )
		{
			throw std::out_of_range( "out of range" );
		}
	}
}

#endif // _TASK5_5_VECTOR_H_

