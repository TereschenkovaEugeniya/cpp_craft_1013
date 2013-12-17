#ifndef _TASK5_5_VECTOR_H_
#define _TASK5_5_VECTOR_H_

#include <cstdlib>
#include <memory>

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
		size_t capacity_;
		size_t size_;
		std::unique_ptr<T[]> buf_;

		static const size_t def_size = 0;
		static const size_t def_capacity = 4;
		static const size_t def_mul = 2;

		void check_ind(const size_t& ind);
	};

	// TODO, please realise the rest methods according to the tests

	template< typename T >
	vector< T >::vector() : capacity_(def_capacity), size_(def_size), buf_(new T[capacity_])
	{
	}
	template< typename T >
	vector< T >::vector( const vector< T >& src )
	{
		*this = src;
	}
	template< typename T >
	vector< T >& vector< T >::operator=( const vector< T >& src )
	{
		this->size_ = src.size_;
		this->capacity_ = src.capacity_;

		this->buf_.reset(new T[this->capacity_]);

		std::copy(src.begin(), src.end(), this->begin());

		return *this;
	}

	template< typename T >
	void vector< T >::push_back( const T& data )
	{
		if (size_ == capacity_)
		{
			reserve(def_mul * capacity_);
		}

		buf_[size_] = data;
		++size_;
	}

	template< typename T >
	void vector< T >::insert( const size_t ind , const T& data )
	{
		check_ind(ind);
		if (size_ == capacity_)
		{
			reserve(def_mul * capacity_);
		}

		for (size_t i = size_; i > ind; --i)
		{
			buf_[i] = buf_[i-1];
		}

		buf_[ind] = data;
		++size_;
	}

	template< typename T >
	T& vector< T >::operator[]( const size_t ind )
	{
		check_ind(ind);
		return buf_[ind];
	}

	template< typename T >
	const T& vector< T >::operator[]( const size_t ind ) const
	{
		check_ind(ind);
		return buf_[ind];
	}

	template< typename T >
	void vector< T >::resize( const size_t new_size )
	{
		if (new_size > size_)
		{
			reserve(new_size);

			for (size_t i = size_; i < new_size; ++i)
			{
				buf_[i] = T();
			}
		}

		size_ = new_size;

	}
	template< typename T >
	void vector< T >::reserve( const size_t new_capacity )
	{
		if (new_capacity > capacity_)
		{
			T* new_buf = new T[new_capacity];

			std::copy(this->begin(), this->end(), new_buf);

			buf_.reset(new_buf);

			capacity_ = new_capacity;
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
		return &buf_[0];
	}
	template< typename T >
	typename vector< T >::iterator vector< T >::end()
	{
		return &buf_[size_];
	}
	
	template< typename T >
	typename vector< T >::const_iterator vector< T >::begin() const
	{
		return &buf_[0];
	}
	template< typename T >
	typename vector< T >::const_iterator vector< T >::end() const
	{
		return (size_ == 0 ? begin() : &buf_[size_ + 1]);
	}

	template< typename T >
	typename void vector< T >::check_ind(const size_t& ind)
	{
		if (ind >= size_)
		{
			throw std::out_of_range("out of range");
		}
	}
}

#endif // _TASK5_5_VECTOR_H_

