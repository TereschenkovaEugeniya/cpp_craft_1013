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
	public:
		explicit thread_safe_queue();
		~thread_safe_queue();

		void push( const T& new_element );
		bool pop( T& result );

		bool empty() const;
		size_t size() const;
    private:
        std::queue<T> in_queue;
        typedef boost::mutex _my_mutex;
        typedef boost::mutex::scoped_lock _lock_mutex;
        mutable _my_mutex mtx_;
	};

	template< typename T >
	thread_safe_queue< T >::thread_safe_queue()
	{
	}

	template< typename T >
	thread_safe_queue< T >::~thread_safe_queue()
	{
	}

	template< typename T >
	void thread_safe_queue< T >::push( const T&  new_element )
	{
        _lock_mutex lock(mtx_);
        in_queue.push(new_element);
	}

	template< typename T >
	bool thread_safe_queue< T >::pop( T& result )
	{
        _lock_mutex lock(mtx_);

        if ( in_queue.empty() )
        {
            return false;
        }

        result = in_queue.front();
        in_queue.pop();

		return true;
	}

	template< typename T >
	bool thread_safe_queue< T >::empty() const
	{
        _lock_mutex lock(mtx_);
		return in_queue.empty();
	}

	template< typename T >
	size_t thread_safe_queue< T >::size() const
	{
        _lock_mutex lock(mtx_);
		return in_queue.size();
	}

}

#endif // _TASK5_6_THREAD_SAFE_QUEUE_H_
