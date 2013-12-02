#ifndef _TASK5_5_VECTOR_H_
#define _TASK5_5_VECTOR_H_

#include <cstdlib>
#include <memory>
#include <stdexcept>

namespace task5_5
{
	template< typename T >
	class vector
	{
		static const size_t DEFAULT_RESERVED_SIZE = 4ul;
		static const size_t DELTA_RESERVED_SIZE = 2ul;
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
		size_t current_size_;
		size_t reserved_size_;
		
		std::unique_ptr<T[]> data_;
	};

	// TODO, please realise the rest methods according to the tests

	template< typename T >
	vector< T >::vector()
	{
		current_size_ = 0ul;
		reserved_size_ = DEFAULT_RESERVED_SIZE;
		data_.reset(new T[reserved_size_]);
	}
	template< typename T >
	vector< T >::vector( const vector< T >&  v)
	{
		*this = v;
	}
	template< typename T >
	vector< T >& vector< T >::operator=( const vector< T >& v  )
	{
		current_size_ = v.current_size_;
		reserved_size_ = v.reserved_size_;
		data_.reset(new T[reserved_size_]);
		if(current_size_ > 0)
			std::copy(v.begin(), v.end(), begin());
		return *this;
	}

	template< typename T >
	void vector< T >::push_back( const T& value)
	{
		if(current_size_ >= reserved_size_)
		{
			reserve(reserved_size_ * DELTA_RESERVED_SIZE);
		}
		
		data_.get()[current_size_] = value;
		current_size_++;
	}

	template< typename T >
	void vector< T >::insert( const size_t index, const T&  value)
	{
		if(index >= current_size_)
		{
			throw std::out_of_range("out of range");
		}

		if(current_size_ >= reserved_size_)
		{
			reserve(reserved_size_ * DELTA_RESERVED_SIZE);
		}

		for(size_t i = current_size_; i > index; i--)
		{
			data_[i] = data_[i - 1];
		}

		data_[index] = value;
		current_size_++;
	}

	template< typename T >
	T& vector< T >::operator[]( const size_t  index )
	{
		if(index >= current_size_)
		{
			throw std::out_of_range("out of range");
		}
		return data_[index];
	}

	template< typename T >
	const T& vector< T >::operator[]( const size_t index ) const
	{
		if(index >= current_size_)
		{
			throw std::out_of_range("out of range");
		}
		return static_cast<const T&>(data_[index]);
	}

	template< typename T >
	void vector< T >::resize( const size_t  new_size)
	{
		if(reserved_size_ < new_size)
		{
			reserve(new_size);
		}

		for(size_t i = current_size_; i < new_size; i++)
		{
			data_[i] = T();
		}
		current_size_ = new_size;

	}

	template< typename T >
	void vector< T >::reserve( const size_t  new_reserved_size)
	{
		if(reserved_size_ < new_reserved_size)
		{
			reserved_size_ = new_reserved_size;
			T *temp_data = new T[new_reserved_size];
			std::copy(begin(), end(), temp_data);
			data_.reset(temp_data);
		}
	}

	template< typename T >
	size_t vector< T >::size() const
	{
		return current_size_;
	}
	template< typename T >
	size_t vector< T >::capacity() const
	{
		return reserved_size_;
	}
	template< typename T >
	bool vector< T >::empty() const
	{
		return false;
	}
	template< typename T >
	typename vector< T >::iterator vector< T >::begin()
	{
		return data_.get();
	}
	template< typename T >
	typename vector< T >::iterator vector< T >::end()
	{
		return data_.get() + current_size_;
	}
	
	template< typename T >
	typename vector< T >::const_iterator vector< T >::begin() const
	{
		return const_cast<const_iterator>(data_.get());
	}

	template< typename T >
	typename vector< T >::const_iterator vector< T >::end() const
	{
		return const_cast<const_iterator>(data_.get() + current_size_);
	}
}

#endif // _TASK5_5_VECTOR_H_

