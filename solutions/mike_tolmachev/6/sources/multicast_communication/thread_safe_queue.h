#ifndef _TASK5_6_THREAD_SAFE_QUEUE_H_
#define _TASK5_6_THREAD_SAFE_QUEUE_H_

#include <cstdlib>
#include <boost/thread.hpp>
#include <queue>

namespace task5_6
{

template< typename T >
class thread_safe_queue
{
	std::queue<T> queue_;
	mutable boost::mutex mtx_;

public:
	explicit thread_safe_queue();
	~thread_safe_queue();

	void push(const T& element);
	bool pop(T& element);
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
void thread_safe_queue< T >::push(const T&  elem)
{
	boost::mutex::scoped_lock lock(mtx_);
	queue_.push(elem);
}

template< typename T >
bool thread_safe_queue< T >::pop(T& elem)
{
	boost::mutex::scoped_lock lock(mtx_);
	if( queue_.empty() )
	{
		return false;
	}
	elem = queue_.front();
	queue_.pop();

	return true;
}

}

#endif // _TASK5_6_THREAD_SAFE_QUEUE_H_
