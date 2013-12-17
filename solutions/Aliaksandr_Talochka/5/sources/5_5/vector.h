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
		~vector();
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
		T* StartPos;
		T* EndPos;
		T* ReservedPos;
	};

	// TODO, please realise the rest methods according to the tests

	template< typename T >
	vector< T >::vector()
	{
		StartPos = new T[4];
		EndPos = StartPos;
		ReservedPos = StartPos + 4;
	}

	template< typename T >
	vector< T >::~vector()
	{
		delete [] StartPos;
	}


	template< typename T >
	vector< T >::vector( const vector< T >&  copy)
	{
		StartPos = 0;
		EndPos = 0;
		ReservedPos = 0;

		*this = copy;
	}

	template< typename T >
	vector< T >& vector< T >::operator=( const vector< T >& copy_from)
	{
		if (&copy_from == this)
			return *this;

		delete [] StartPos;

		StartPos = new T[copy_from.capacity()];
		ReservedPos = StartPos + copy_from.capacity();
		EndPos = StartPos + copy_from.size();	

		for (size_t i = 0; i < copy_from.size(); i++)
		{
			*(StartPos + i) = copy_from[i];
		}

		return *this;
	}

	template< typename T >
	void vector< T >::push_back( const T& value)
	{
		if (EndPos == ReservedPos)
			this->reserve(this->capacity()*2);


		*(EndPos) = value;
		EndPos++;
	}

	template< typename T >
	void vector< T >::insert( const size_t index, const T& value )
	{
		if( index >= size() )
			throw std::out_of_range("incorrect index");

		resize(size()+1);

		for(iterator it = EndPos-1; it > StartPos + index; it--)
		{
			*it = *(it-1);
		}

		*(StartPos+ index) = value;

	}

	template< typename T >
	T& vector< T >::operator[]( const size_t index)
	{
		if( index >= this->size() )
			throw std::out_of_range("incorrect index");

		return *(StartPos + index);
	}

	template< typename T >
	const T& vector< T >::operator[]( const size_t  index) const
	{
		if( index >= this->size() )
			throw std::out_of_range("incorrect index");

		return *(StartPos + index);
	}

	template< typename T >
	void vector< T >::resize( const size_t  amount)
	{
		if( amount > capacity())
			reserve(amount);

		if ((size_t)(EndPos - StartPos)<amount)
		{
			size_t num = (size_t)(ReservedPos - EndPos);
			memset(EndPos, 0, num*sizeof(T));
		}
		EndPos = StartPos + amount;

	}
	template< typename T >
	void vector< T >::reserve( const size_t  amount)
	{
		if (amount > capacity())
		{
			T* tempStartPos;
			const size_t sz = size();

			tempStartPos = new T[amount];
			memcpy(tempStartPos, StartPos, sizeof(T)*sz);

			delete [] StartPos;
			StartPos = tempStartPos;
			EndPos = StartPos + sz;
			ReservedPos = StartPos + amount;
		}
	}

	template< typename T >
	size_t vector< T >::size() const
	{
		return EndPos - StartPos;
	}

	template< typename T >
	size_t vector< T >::capacity() const
	{
		return ReservedPos - StartPos;
	}

	template< typename T >
	bool vector< T >::empty() const
	{
		return (size()==0);
	}

	template< typename T >
	typename vector< T >::iterator vector< T >::begin()
	{
		return StartPos;
	}

	template< typename T >
	typename vector< T >::iterator vector< T >::end()
	{
		return EndPos;
	}
	
	template< typename T >
	typename vector< T >::const_iterator vector< T >::begin() const
	{
		return StartPos;
	}

	template< typename T >
	typename vector< T >::const_iterator vector< T >::end() const
	{
		return EndPos;
	}
}

#endif // _TASK5_5_VECTOR_H_

