#ifndef _TASK5_5_VECTOR_H_
#define _TASK5_5_VECTOR_H_

#include <cstdlib>
#include <boost/scoped_array.hpp>
#include <stdexcept>

namespace task5_5
{
    template< typename T >
        class vector
        {
            public:
                typedef T* iterator ; // you could change this
                typedef const T* const_iterator; // you could change this

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
                static size_t const DEFAULT_CAPACITY = 4;
                static size_t const FACTOR = 2;
                size_t capacity_;
                size_t size_;
                boost::scoped_array<T> data_;
                void check_capacity( size_t const capacity, size_t increase_value );
                void copy_from( vector<T> const&);
                void copy_data( boost::scoped_array<T> const& from, boost::scoped_array<T>& to);
        };

    template< typename T >
        vector< T >::vector() 
    { 
        this->size_ = 0;
        this->capacity_ = DEFAULT_CAPACITY;
        this->data_.reset(new T[DEFAULT_CAPACITY]);
    }
    template< typename T >
        vector< T >::vector( const vector< T >& t)
        {
            if(this != &t)
            {
                this->size_ = t.size_;
                this->capacity_ = t.capacity_;
                this->data_.reset(new T[this->capacity_]); 
                copy_from(t);
            }
        }
    template< typename T >
        vector< T >& vector< T >::operator=(const vector<T>& t)
        {
            if(this != &t)
            {
                this->size_ = t.size_;
                check_capacity(t.capacity_, t.capacity_);
                copy_from(t);
            }

            return *this;
        }
    template< typename T >
        void vector< T >::push_back( const T& t)
        {
            check_capacity( this->size_ + 1, this->capacity_ * FACTOR );
            this->data_[ this->size_++ ] = t;
        }

    template< typename T >
        void vector< T >::insert( const size_t index, const T& t )
        {
            check_capacity( this->size_ + 1, this->capacity_ * FACTOR );
            ++this->size_;
            for(size_t i = this->size_; i > index ; --i)
                this->data_[ i ] = this->data_[ i - 1 ]; 
            this->data_[ index ] = t;
        }

    template< typename T >
        T& vector< T >::operator[]( const size_t index )
        {
            if( index >= this->size_)
                throw std::out_of_range("operator[]: out of range.");
            return this->data_[index];
        }

    template< typename T >
        const T& vector< T >::operator[]( const size_t index ) const
        {
            if( index >= this->size_)
                throw std::out_of_range("operator[]: out of range.");
            return this->data_[index];
        }

    template < typename T >
        void vector<T>::check_capacity( size_t const amount, size_t const increase_value )
        {
            if(amount > this->capacity_)
            {
                this->capacity_ = increase_value;
                boost::scoped_array<T> copy(new T[this->size_]);
                copy_data(this->data_, copy);
                this->data_.reset(new T[this->capacity_]);
                copy_data(copy, this->data_);
            }
        }

    template< typename T >
        void vector< T >::resize( const size_t amount)
        { 
            check_capacity( amount, amount );
            this->size_ = amount;
            for(size_t i = this->size_; i < this->capacity_; ++i)
            {
                this->data_[i] = T();
            }
        }
    template< typename T >
        void vector< T >::reserve( const size_t new_capacity )
        {
            check_capacity(new_capacity, new_capacity);
        }

    template< typename T >
        void vector<T>::copy_from( vector<T> const& v)
        { 
            copy_data(v.data_, this->data_);
        }
    template< typename T >
        void vector<T>::copy_data( boost::scoped_array<T> const& from, boost::scoped_array<T>& to)
        { 
            for(size_t i = 0; i < this->size_; ++i)
                to[i] = from[i];
        }

    template< typename T >
        size_t vector< T >::size() const
        {
            return this->size_;
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
            return &(this->data_[0]);
        }
    template< typename T >
        typename vector< T >::iterator vector< T >::end()
        {
            return &(this->data_[this->size_]);
        }

    template< typename T >
        typename vector< T >::const_iterator vector< T >::begin() const
        {
            return &(this->data_[0]);
        }
    template< typename T >
        typename vector< T >::const_iterator vector< T >::end() const
        {
            return &(this->data_[this->size_]);
        }
}

#endif // _TASK5_5_VECTOR_H_ 
