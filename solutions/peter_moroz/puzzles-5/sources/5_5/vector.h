#ifndef _TASK5_5_VECTOR_H_
#define _TASK5_5_VECTOR_H_

#include <cstdlib>
#include <stdexcept>

namespace task5_5
{
	template< typename T >
	class vector
	{
    static const size_t kInitialCapacity = 4;
    static const size_t kGrowScale = 2;
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
  private:
    void grow();

  private:
    T* data_;
    size_t size_;
    size_t capacity_;
	};

	// TODO, please realise the rest methods according to the tests

	template< typename T >
  vector< T >::vector() : data_(NULL), size_(0), capacity_(0)
	{
    // one additional element at the end means 'end of vector'
    data_ = new T[kInitialCapacity + 1]();
    capacity_ = kInitialCapacity;
	}
	template< typename T >
	vector< T >::vector( const vector< T >& copy )
	{
    data_ = NULL;
    size_ = 0;
    capacity_ = 0;

    T* data = new T[copy.capacity() + 1]();

    T* dst = data;
    T* src = copy.data_;
    for (size_t i = 0; i < copy.size(); ++i) 
    {
      try {
        *dst = *src;
        ++dst;
        ++src;
      } catch (...) {
        delete [] data;
        throw;
      }
    }
    data_ = data;
    size_ = copy.size();
    capacity_ = copy.capacity();
	}
	template< typename T >
	vector< T >& vector< T >::operator=( const vector< T >& copy_from )
	{
    if (this != &copy_from)
    {
      T* data = new T[copy_from.capacity() + 1]();
      T* dst = data;
      T* src = copy_from.data_;
      for (size_t i = 0; i < copy_from.size(); ++i) 
      {
        try {
          *dst = *src;
        } catch (...) {
          delete [] data;
          throw;
        }
        ++dst;
        ++src;
      }
      delete [] data_;
      data_ = data;
      size_ = copy_from.size();
      capacity_ = copy_from.capacity();
    }
		return *this;
	}

  template< typename T >
  vector< T >::~vector()
  {
    delete [] data_;
    data_ = NULL;
    size_ = 0;
    capacity_ = 0;
  }
	template< typename T >
	void vector< T >::push_back( const T& value )
	{
    if (size() + 1 > capacity())
      grow();
    data_[size()] = value;
    ++size_;
	}

	template< typename T >
	void vector< T >::insert( const size_t index, const T& value )
	{
    if (size() + 1 > capacity())
      grow();
      

    T* data = new T[capacity() + 1];
    T* src = data_;
    T* dst = data;

    size_t n = size() + 1;
    for (size_t i = 0; i < n; ++i)
    {
      try {
        if (i == index)
          *dst = value;
        else
        {
          *dst = *src;
          ++src;
        }
      } catch (...) {
        delete [] data;
        throw;
      }
      ++dst;
    }

    delete [] data_;
    data_ = data;

    ++size_;
	}

	template< typename T >
	T& vector< T >::operator[]( const size_t index )
	{
    if ((index + 1) > size())
      throw std::out_of_range("task5_5::vector::operator[]");
		return *(data_ + index);
	}

	template< typename T >
	const T& vector< T >::operator[]( const size_t index ) const
	{
    if ((index + 1) > size())
      throw std::out_of_range("task5_5::vector::operator[]");
		return *(data_ + index);
	}

	template< typename T >
	void vector< T >::resize( const size_t amount )
	{
    if (amount == size())
      return;
    if (amount < size())
    {
      T* p = data_ + amount;
      size_t count = size() - amount;
      while (count > 0)
      {
        p->~T();
        ++p;
        --count;
      }
    } 
    else
    {
      if (amount > capacity())
        reserve(amount);
    }
    size_ = amount;
	}
	template< typename T >
	void vector< T >::reserve( const size_t amount )
	{
    if (amount > capacity())
    {
      // one additional element at the end means 'end of vector'
      T* data = new T[amount + 1]();
      T* src = data_;
      T* dst = data;
      size_t count = size();
      while (count > 0)
      {
        try {
          *dst = *src;
        } catch (...) {
          delete [] data;
          throw;
        }
        ++dst;
        ++src;
        --count;
      }
       
      delete [] data_;
      data_ = data;
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
		return (size() == 0);
	}
	template< typename T >
	typename vector< T >::iterator vector< T >::begin()
	{
		return data_;
	}
	template< typename T >
	typename vector< T >::iterator vector< T >::end()
	{
		return (data_ + size());
	}
	
	template< typename T >
	typename vector< T >::const_iterator vector< T >::begin() const
	{
		return data_;
	}
	template< typename T >
	typename vector< T >::const_iterator vector< T >::end() const
	{
		return (data_ + size());
	}
  template< typename T >
  void vector<T>::grow()
  {
    reserve(capacity() * kGrowScale);
  }
}

#endif // _TASK5_5_VECTOR_H_

