#ifndef _TASK5_5_VECTOR_H_
#define _TASK5_5_VECTOR_H_

#include <cstdlib>
#include <stdexcept>

namespace task5_5
{
	template< typename T >
	class vector
	{
	private:
		T* VECTOR;
		size_t length_;
		size_t capacity_;
		
	public:
		static const size_t increase_mem;
		typedef T* iterator ; // you could change this
		typedef const T* const_iterator; // you could change this


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
	};

	template< typename T >
	const size_t vector<T> :: increase_mem = 2;

	template< typename T >
	vector< T >::vector()
	{
		length_ = 0;
		capacity_ = 4;
		VECTOR = new T [capacity_];
	}

	
	template< typename T >
	vector< T >::vector( const vector< T >&  copy)
	{
		length_ = copy.size();
		capacity_ = copy.capacity();
		VECTOR = new T[capacity_];
		for(size_t i=0;i<length_;i++)
			VECTOR[i] = copy[i];

	}
	template< typename T >
	vector< T >& vector< T >::operator=( const vector< T >&  copy_from)
	{
		length_ = copy_from.size();
		capacity_ = copy_from.capacity();
		delete[] VECTOR;
		VECTOR = new T[capacity_];
		for(size_t i=0;i<length_;i++)
			VECTOR[i] = copy_from [i];
		return *this;
	}

	template< typename T >
	void vector< T >::push_back( const T& new_element)
	{
		if (capacity_ <= length_)		
		{
			capacity_*=increase_mem;
			T * temp = new T[capacity_];
			for(size_t i=0;i<length_;i++)
				temp[i]=VECTOR[i];
			delete[] VECTOR;			
			temp[length_] = new_element;
			length_++;
			VECTOR = temp;
			return;
		}
		VECTOR [length_] = new_element;
		length_++;
	}

	template< typename T >
	vector<T>::~vector()
	{
		delete[] VECTOR;
	}


	template< typename T >
	void vector< T >::insert( const size_t place, const T& new_element)
	{
		if (place>length_) 
			throw std::out_of_range("Error in insert");
		if (length_>=capacity_)
		{
			reserve(capacity_*increase_mem);
		}
		size_t i = length_;
		while(i!=place)
		{
			VECTOR[i] = VECTOR[i-1];
			i--;
		}
		length_++;
		VECTOR[place]=new_element;
	}

	template< typename T >
	T& vector< T >::operator[]( const size_t  i)
	{
		if (i>=length_ || i<0) 
		{
			throw std :: out_of_range("Error in []");			
		}
		return VECTOR[i];
	}

	template< typename T >
	const T& vector< T >::operator[]( const size_t i ) const
	{
		if (i>=length_ || i<0) 
		{
			throw std :: out_of_range("Error in const[]");
		}
		return VECTOR[i];
	}

	template< typename T >
	void vector< T >::resize( const size_t  new_length)
	{
		if (new_length<capacity_)
		{
			length_ = new_length;
			for(size_t i= length_;i<capacity_;i++)
				VECTOR[i] = T();
			return;
		}
		capacity_ = new_length;		
		T* temp = new T[capacity_];
		for(size_t i=0;i<length_;i++)
			temp[i]=VECTOR[i];
		for(size_t i=length_;i<capacity_;i++)
			temp[i] = T();	
		length_ = capacity_;
		delete[] VECTOR;
		VECTOR = temp;	
	}
	template< typename T >
	void vector< T >::reserve( const size_t new_capacity )
	{
		if (new_capacity < capacity_) return;
		capacity_ = new_capacity;
		T * temp = new T[new_capacity];
		for(size_t i=0;i<length_;i++)
			temp[i] = VECTOR[i];
		delete[] VECTOR;
		VECTOR = temp;
	}

	template< typename T >
	size_t vector< T >::size() const
	{
		return length_;
	}
	template< typename T >
	size_t vector< T >::capacity() const
	{
		return capacity_;
	}
	template< typename T >
	bool vector< T >::empty() const
	{
		return length_==0;
	}
	template< typename T >
	typename vector< T >::iterator vector< T >::begin()
	{
		return &VECTOR[0];
	}
	template< typename T >
	typename vector< T >::iterator vector< T >::end()
	{
		return &VECTOR[length_];
	}
	
	template< typename T >
	typename vector< T >::const_iterator vector< T >::begin() const
	{
		return &VECTOR[0];
	}
	template< typename T >
	typename vector< T >::const_iterator vector< T >::end() const
	{
		return &VECTOR[length_];
	}
}

#endif // _TASK5_5_VECTOR_H_

