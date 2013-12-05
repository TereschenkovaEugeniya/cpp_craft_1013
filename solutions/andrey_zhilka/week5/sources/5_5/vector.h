#ifndef _TASK5_5_VECTOR_H_
#define _TASK5_5_VECTOR_H_

#include <cstdlib>

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
		~vector();

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

	private :
		T* content_;
		iterator end_iter_;
		iterator begin_iter_; 
		size_t capacity_;
		void refill_content( const vector< T >&, bool );
		void refresh_content( size_t size );
	};

	// TODO, please realise the rest methods according to the tests

	template< typename T >
	void vector< T >::refresh_content( size_t old_size )
	{
		T* buffer = new T[ capacity_ + 1 ];
		T default_val = T();
		std::fill_n( buffer, capacity_, default_val );
		memcpy( buffer, content_, old_size * sizeof( T ) );
		delete [] content_;
		content_ = buffer;
		begin_iter_ = content_;
		end_iter_ = &content_[old_size];
	}

	template< typename T >
	void vector< T >::refill_content( const vector< T >& to_copy, bool clean )
	{
		if ( clean )
		{
			delete [] content_;
		}
		capacity_ = to_copy.capacity_;
		content_ = new T[to_copy.capacity_ + 1];
		memcpy( content_, to_copy.content_, capacity_ * sizeof( T ) );
		begin_iter_ = content_;
		end_iter_ = &content_[to_copy.size()];
	}

	template< typename T >
	vector< T >::vector()
	{
		capacity_ = 4ul;
		content_ = new T[capacity_ + 1];
		begin_iter_ = content_;
		end_iter_ = begin_iter_;
		
	}
	template< typename T >
	vector< T >::vector( const vector< T >& to_copy )
	{
		refill_content( to_copy, false );
	}

	template< typename T >
	vector< T >::~vector()
	{
		delete [] content_;
	}

	template< typename T >
	vector< T >& vector< T >::operator=( const vector< T >&  to_copy)
	{
		refill_content( to_copy, true );
		return *this;
	}

	template< typename T >
	void vector< T >::push_back( const T& element)
	{
		size_t cur_size = this->size();
		if ( capacity_ == cur_size )
		{
			capacity_ *= 2;
			refresh_content( cur_size );
		}

		*end_iter_ = element;
		end_iter_ = &content_[cur_size + 1];
	}

	template< typename T >
	void vector< T >::insert( const size_t pos, const T& element )
	{
		for ( size_t i = size(); i > pos; i-- )
		{
			content_[i] = content_[i-1];
		}
		content_[pos] = element;
		end_iter_ = &content_[size() + 1];
	}

	template< typename T >
	T& vector< T >::operator[]( const size_t  index )
	{
		if ( index >= this->size() )
		{
			throw std::out_of_range("Index out of range");
		}

		return *(begin_iter_ + index);
	}

	template< typename T >
	const T& vector< T >::operator[]( const size_t index ) const
	{
		if ( index > this->size() )
		{
			throw std::out_of_range("Index is out of range");
		}
		const T el = *(begin_iter_ + index);
		return el;
	}

	template< typename T >
	void vector< T >::resize( const size_t new_size )
	{
		if ( capacity_ < new_size )
		{
			capacity_ = new_size;
		}

		refresh_content( std::min( this->size(), new_size ) );
		end_iter_ = &content_[new_size];
	}

	template< typename T >
	void vector< T >::reserve( const size_t new_capacity )
	{
		if ( capacity_ < new_capacity )
		{
			capacity_ = new_capacity;
			refresh_content( this->size() );
		}
	}

	template< typename T >
	size_t vector< T >::size() const
	{
		return (end_iter_ - begin_iter_);
	}
	template< typename T >
	size_t vector< T >::capacity() const
	{
		return capacity_;
	}
	template< typename T >
	bool vector< T >::empty() const
	{
		return (begin_iter_ == end_iter_);
	}
	template< typename T >
	typename vector< T >::iterator vector< T >::begin()
	{
		return begin_iter_;
	}
	template< typename T >
	typename vector< T >::iterator vector< T >::end()
	{
		return end_iter_;
	}
	
	template< typename T >
	typename vector< T >::const_iterator vector< T >::begin() const
	{
		const_iterator b = begin_iter_;
		return b;
	}
	template< typename T >
	typename vector< T >::const_iterator vector< T >::end() const
	{
		const_iterator e = end_iter_;
		return e;
	}
}

#endif // _TASK5_5_VECTOR_H_

