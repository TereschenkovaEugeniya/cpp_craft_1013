#ifndef _TASK5_6_THREAD_SAFE_QUEUE_H_
#define _TASK5_6_THREAD_SAFE_QUEUE_H_

#include <cstdlib>
#include <boost/thread.hpp>

namespace task5_6
{
	template< typename T >
	class thread_safe_queue
	{
		size_t elements_cnt;
		mutable boost::mutex size_protector, front_protector, rear_protector, push_protector, pop_protector;

		struct element
		{
			T value;
			element * next;

			element( int value )
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
	};

	template< typename T >
	thread_safe_queue< T >::thread_safe_queue()
		:elements_cnt( 0 ), front( NULL ), rear( NULL )
	{}

	template< typename T >
	thread_safe_queue< T >::~thread_safe_queue()
	{
		T a;
		while( !empty() )
			pop(a);
	}

	template< typename T >
	void thread_safe_queue< T >::push( const T& value )
	{
		boost::mutex::scoped_lock function_lock( push_protector );
		element *temp = new element( value );

		{
			boost::mutex::scoped_lock lock_rear( rear_protector );

			if( rear == NULL )
			{
				boost::mutex::scoped_lock lock_front( front_protector );
				front = rear = temp;
			}
			else
			{
				rear->next = temp;
				rear = temp;
			}
		}

		boost::mutex::scoped_lock lock( size_protector );
		++elements_cnt;
	}

	template< typename T >
	bool thread_safe_queue< T >::pop( T& var )
	{
		boost::mutex::scoped_lock function_lock( pop_protector );
		
		if( empty() )
			return false;
		{
			boost::mutex::scoped_lock lock_front( front_protector );
			var = front->value;
		}
		{
			boost::mutex::scoped_lock lock_rear( rear_protector ), lock_front( front_protector );
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

}

#endif // _TASK5_6_THREAD_SAFE_QUEUE_H_
