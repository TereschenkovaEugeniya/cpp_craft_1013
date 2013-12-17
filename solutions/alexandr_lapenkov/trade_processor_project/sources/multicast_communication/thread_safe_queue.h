#ifndef _TASK5_6_THREAD_SAFE_QUEUE_H_
#define _TASK5_6_THREAD_SAFE_QUEUE_H_

#include <cstdlib>

#include <boost/thread.hpp>

namespace multicast_communication
{
	template< typename T >
	class thread_safe_queue
	{
		size_t elements_cnt;
		mutable boost::mutex size_protector, push_pop_protector;
		bool interrupt_;
		boost::condition_variable cond;

		struct element
		{
			T value;
			element * next;

			element( T value )
				:value( value ), next( NULL )
			{}
		};

		element *front, *rear;

	public:
		explicit thread_safe_queue();
		~thread_safe_queue();

		void push( const T& new_element );
		bool pop( T& result );

		bool empty() const;
		size_t size() const;
		void interrupt();
	};

	template< typename T >
	thread_safe_queue< T >::thread_safe_queue()
		:elements_cnt( 0 ), front( NULL ), rear( NULL ), interrupt_( false )
	{}

	template< typename T >
	thread_safe_queue< T >::~thread_safe_queue()
	{
		boost::mutex::scoped_lock lock( push_pop_protector );
		for( element* a = rear; a != NULL; )
		{
			element *next = a->next;
			delete a;
			a = next;
		}
	}

	template< typename T >
	void thread_safe_queue< T >::push( const T& value )
	{
		boost::mutex::scoped_lock function_lock( push_pop_protector );
		element *temp = new element( value );

		if( rear == NULL )
		{
			front = rear = temp;
		}
		else
		{
			rear->next = temp;
			rear = temp;
		}

		boost::mutex::scoped_lock lock( size_protector );
		++elements_cnt;

		cond.notify_one();
	}

	template< typename T >
	bool thread_safe_queue< T >::pop( T& var )
	{
		boost::mutex::scoped_lock function_lock( push_pop_protector );
		
		while( empty() && !interrupt_ )
			cond.wait( function_lock );
	
		if( interrupt_ )
			return false;

		var = front->value;

		if( front == rear )
		{
			delete front;
			front = rear = NULL;
		}
		else 
		{
			element *move_to = front->next;
			delete front;
			front = move_to;
		}

		boost::mutex::scoped_lock lock( size_protector );
		--elements_cnt;

		return true;
	}

	template< typename T >
	bool thread_safe_queue< T >::empty() const
	{
		return ( size() == 0 );
	}

	template< typename T >
	size_t thread_safe_queue< T >::size() const
	{
		boost::mutex::scoped_lock lock( size_protector );
		return elements_cnt;
	}

	template< typename T >
	void thread_safe_queue< T >::interrupt()
	{
		boost::mutex::scoped_lock function_lock( push_pop_protector );
		interrupt_ = true;
		function_lock.unlock();
		cond.notify_all();
	}

}

#endif // _TASK5_6_THREAD_SAFE_QUEUE_H_
