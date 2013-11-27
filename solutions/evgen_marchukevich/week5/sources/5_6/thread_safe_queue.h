#ifndef _TASK5_6_THREAD_SAFE_QUEUE_H_
#define _TASK5_6_THREAD_SAFE_QUEUE_H_

#include <cstdlib>
#include <queue>
#include <boost/thread.hpp>


namespace task5_6
{
	template< typename T >
	class thread_safe_queue
	{
	private:
		size_t q_size;
		mutable boost :: mutex mtx_size, mtx_queue;
		std::queue<T> QUEUE;

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
	{
		q_size=0;
	}

	template< typename T >
	thread_safe_queue< T >::~thread_safe_queue()
	{		
		boost :: mutex :: scoped_lock Lock_size(mtx_size);
		boost :: mutex :: scoped_lock Lock_pop(mtx_queue);
		while (q_size!=0)
		{
			QUEUE.pop();
			q_size--;
		}
	}

	template< typename T >
	void thread_safe_queue< T >::push( const T&  new_element)
	{
		{
			boost :: mutex :: scoped_lock Lock_push(mtx_queue);			
			QUEUE.push(new_element);			
		}
		{
			boost :: mutex :: scoped_lock Lock_size(mtx_size);	
			q_size++;			
		}
	}

	template< typename T >
	bool thread_safe_queue< T >::pop( T& result)
	{
		{
			boost :: mutex :: scoped_lock Lock(mtx_size);
			boost :: mutex :: scoped_lock Lock_queue(mtx_queue);
			if (q_size==0 || QUEUE.empty()) return false;
		}		
		{
			boost :: mutex :: scoped_lock Lock_pop(mtx_queue);
			result= QUEUE.front();
			QUEUE.pop();			
		}
		{
			boost :: mutex :: scoped_lock Lock_size(mtx_size);
			q_size--;		
		}
		return true;
	}

	template< typename T >
	bool thread_safe_queue< T >::empty() const
	{
		boost :: mutex :: scoped_lock Lock(mtx_size);
		return q_size==0;
	}

	template< typename T >
	size_t thread_safe_queue< T >::size() const
	{
		boost :: mutex :: scoped_lock Lock(mtx_size);
		return q_size;
	}

}

#endif // _TASK5_6_THREAD_SAFE_QUEUE_H_
