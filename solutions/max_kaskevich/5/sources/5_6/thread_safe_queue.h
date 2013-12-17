#ifndef _TASK5_6_THREAD_SAFE_QUEUE_H_
#define _TASK5_6_THREAD_SAFE_QUEUE_H_

#include <cstdlib>
#include <memory>
#include <boost/thread.hpp>

namespace task5_6
{

	template< typename T >
	class thread_safe_queue
	{
        struct node
        {
            std::shared_ptr< node > next;
            const T data;
            node( const T& t ) :
                data( t )
            {

            }
        };

        std::shared_ptr< node > first_;
        std::shared_ptr< node > last_;
        size_t size_;
        boost::mutex mtx_;

	public:
		explicit thread_safe_queue();
		~thread_safe_queue();

		void push( const T& new_element );
		bool pop( T& result );

		bool empty() const;
		size_t size() const;
	};

	template< typename T >
	thread_safe_queue< T >::thread_safe_queue() :
        first_( nullptr ),
        last_( nullptr ),
        size_( 0 )
	{
	}

	template< typename T >
	thread_safe_queue< T >::~thread_safe_queue()
	{
	}

	template< typename T >
	void thread_safe_queue< T >::push( const T&  elem )
	{
        boost::mutex::scoped_lock lock(mtx_);
        std::shared_ptr< node > push_node( new node( elem ) );
        if ( last_ )
        {
            last_->next = push_node;
        }
        else
        {
            first_ = push_node;
        }
        last_ = push_node;
        ++size_;
	}

	template< typename T >
	bool thread_safe_queue< T >::pop( T& elem )
	{
        boost::mutex::scoped_lock lock(mtx_);
        if( first_ )
        {
            elem = first_->data;
            first_ = first_->next;
            if( !first_ )
            {
                last_.reset();
            }
            --size_;
            return true;
        }
		return false;
	}

	template< typename T >
	bool thread_safe_queue< T >::empty() const
	{
		return !size_;
	}

	template< typename T >
	size_t thread_safe_queue< T >::size() const
	{
		return size_;
	}

}

#endif // _TASK5_6_THREAD_SAFE_QUEUE_H_
