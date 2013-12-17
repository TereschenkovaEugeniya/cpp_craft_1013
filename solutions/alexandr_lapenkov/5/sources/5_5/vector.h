#ifndef _TASK5_5_VECTOR_H_
#define _TASK5_5_VECTOR_H_

#include <cstdlib>
#include <iterator>
#include <exception>

namespace task5_5
{
	template< typename T >
	class vector
	{
		const static unsigned k = 2u; 

		size_t elements_cnt;
		size_t allocated;
		T *elements;

		void reallocate( size_t size );

	public:

		class iterator : public std::iterator< std::random_access_iterator_tag, T >
		{
			size_t shift;
			vector< T >& parent;
			template< typename U > friend class vector; 

		public:

			explicit iterator()
			{}

			explicit iterator( vector< T >& parent, const size_t shift_to ):
				parent( parent ), shift( shift_to )
			{}

			iterator( const iterator& from ):
				parent( from.parent ), shift( from.shift )
			{
			}

			bool operator!= ( const iterator & to ) const
			{
				return ( !( this->operator== ( to ) ) );
			}

			bool operator== ( const iterator & to ) const
			{
				return ( ( &parent ) == ( &to.parent ) && shift == to.shift );
			}

			bool operator< ( const iterator & to ) const
			{
				return ( shift < to.shift );
			}

			bool operator> ( const iterator & to ) const
			{
				return ( shift > to.shift );
			}

			iterator& operator= ( const iterator& from )
			{
				parent = from.parent;
				shift = from.shift;
				return *this;
			}

			T& operator* ()
			{
				return parent[ shift ];
			}

			iterator& operator++ ()
			{
				++shift;
				return *this;
			}

			iterator& operator++ ( int )
			{
				iterator old( *this );
				++shift;
				return old;
			}

			iterator& operator-- ()
			{
				--shift;
				return *this;
			}

			iterator& operator-- ( int )
			{
				iterator old( *this );
				--shift;
				return old;
			}

			const iterator operator+ ( const int a ) const
			{
				return iterator( parent, shift + a );
			}

			const iterator operator- ( const int a ) const
			{
				return iterator( parent, shift - a );
			}

		};

		class const_iterator : public std::iterator< std::bidirectional_iterator_tag, T >
		{
			size_t shift;
			const vector< T >& parent;
			template< typename U > friend class vector; 

		public:

			explicit const_iterator()
			{}

			explicit const_iterator( const vector< T >& parent, const size_t shift_to ):
				parent( parent ), shift( shift_to )
			{}

			const_iterator( const const_iterator& from ):
				parent( from.parent ), shift( from.shift )
			{}

			bool operator!= ( const const_iterator & to ) const
			{
				return ( !( this->operator== ( to ) ) );
			}

			bool operator== ( const const_iterator & to ) const
			{
				return ( ( &parent ) == ( &to.parent ) && shift == to.shift );
			}

			bool operator< ( const const_iterator & to ) const
			{
				return ( shift < to.shift );
			}

			bool operator> ( const const_iterator & to ) const
			{
				return ( shift > to.shift );
			}

			const_iterator& operator= ( const const_iterator& from )
			{
				parent = from.parent;
				shift = from.shift;
				return *this;
			}

			const T& operator* ()
			{
				return parent[ shift ];
			}

			const_iterator& operator++ ()
			{
				++shift;
				return *this;
			}

			const_iterator& operator++ ( int )
			{
				const_iterator old( *this );
				++shift;
				return old;
			}

			const_iterator& operator-- ()
			{
				--shift;
				return *this;
			}

			const_iterator& operator-- ( int )
			{
				const_iterator old( *this );
				--shift;
				return old;
			}

			const const_iterator operator+ ( const int a ) const
			{
				return const_iterator( parent, shift + a );
			}

			const const_iterator operator- ( const int a ) const
			{
				return const_iterator( parent, shift - a );
			}
		};

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
	};

	// TODO, please realise the rest methods according to the tests

	
	template <typename T>
	void task5_5::vector< T >::reallocate( size_t size )
	{
		const size_t edge = std::min( size, elements_cnt );

		T *buffer = new T[ elements_cnt ];

		for( size_t i = 0; i < edge; ++i )
			buffer[ i ] = elements[ i ];
	
		delete [] elements;
		elements = new T[ size ];
	
		for( size_t i = 0; i < edge; ++i )
			elements[ i ] = buffer[ i ];

		allocated = size;

		delete [] buffer; 
	}

	template< typename T >
	vector< T >::vector():
		elements_cnt( 0 ), allocated( 4 )
	{
		elements = new T[4];	
	}

	template< typename T >
	vector< T >::vector( const vector< T >& from )
	{
		elements = new int[ from.allocated ];
		for( size_t i = 0; i < from.elements_cnt; ++i )
			elements[ i ] = from.elements [ i ];
		allocated = from.allocated;
		elements_cnt = from.elements_cnt;
	}

	template< typename T >
	vector< T >& vector< T >::operator=( const vector< T >& from )
	{
		delete [] elements;
		elements = new int[ from.allocated ];
		for( size_t i = 0; i < from.elements_cnt; ++i )
			elements[ i ] = from.elements [ i ];
		allocated = from.allocated;
		elements_cnt = from.elements_cnt;
		return *this;
	}

	template< typename T >
	void vector< T >::push_back( const T& variable )
	{
		if(elements_cnt == allocated)
			reallocate( allocated*k );
		
		elements[ elements_cnt++ ] = variable; 
	}

	template< typename T >
	void vector< T >::insert( const size_t position, const T& variable )
	{

		if( position >= elements_cnt )
			return ;

		if(elements_cnt == allocated)
			reallocate( allocated*k );

		for( size_t i = elements_cnt; i > position; --i )
			elements[ i ] = elements[ i-1 ];

		elements[ position ] = variable;

		++elements_cnt;
	}

	template< typename T >
	T& vector< T >::operator[]( const size_t position )
	{
		if( position >= elements_cnt )
			throw std::out_of_range( "No such element with such index" );

		return elements[ position ];
	}

	template< typename T >
	const T& vector< T >::operator[]( const size_t position ) const
	{
		if( position >= elements_cnt )
			throw std::out_of_range( "No such element with such index" );

		return elements[ position ];
	}

	template< typename T >
	void vector< T >::resize( const size_t size )
	{
		if( size < 0 )
			throw std::logic_error( "Size can't be negative" );

		if( size > allocated )
			reallocate( size );

		for( size_t i = elements_cnt; i < size; ++i )
			elements[ i ] = T();

		elements_cnt = size;

	}

	template< typename T >
	void vector< T >::reserve( const size_t size )
	{
		if( size > allocated )
		{
			reallocate( size );
		}

	}

	template< typename T >
	size_t vector< T >::size() const
	{
		return elements_cnt;
	}

	template< typename T >
	size_t vector< T >::capacity() const
	{
		return allocated;
	}

	template< typename T >
	bool vector< T >::empty() const
	{
		return ( elements == 0 );
	}

	template< typename T >
	typename vector< T >::iterator vector< T >::begin()
	{
		return vector< T >::iterator( *this, 0 );
	}

	template< typename T >
	typename vector< T >::iterator vector< T >::end()
	{
		return vector< T >::iterator( *this, elements_cnt );
	}
	
	template< typename T >
	typename vector< T >::const_iterator vector< T >::begin() const
	{
		return vector< T >::const_iterator( *this, 0 );
	}

	template< typename T >
	typename vector< T >::const_iterator vector< T >::end() const
	{
		return vector< T >::const_iterator( *this, elements_cnt );
	}

	template< typename T >
	vector< T >::~vector()
	{
		delete [] elements;
	}

}

#endif // _TASK5_5_VECTOR_H_

