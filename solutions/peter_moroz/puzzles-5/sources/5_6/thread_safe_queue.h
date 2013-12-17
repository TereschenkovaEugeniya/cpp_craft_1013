#ifndef _TASK5_6_THREAD_SAFE_QUEUE_H_
#define _TASK5_6_THREAD_SAFE_QUEUE_H_

#include <cstdlib>
#include <list>
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
  public:
    std::list<T> container_;
    mutable boost::recursive_mutex container_guard_;
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
	void thread_safe_queue< T >::push( const T& new_element )
	{
    boost::unique_lock<boost::recursive_mutex> lock(container_guard_);
    container_.push_back( new_element );
	}

	template< typename T >
	bool thread_safe_queue< T >::pop( T& result )
	{
    boost::unique_lock<boost::recursive_mutex> lock(container_guard_);
    if (empty())
      return false;

    result = container_.front();
    container_.pop_front();
		return true;
	}

	template< typename T >
	bool thread_safe_queue< T >::empty() const
	{
    boost::unique_lock<boost::recursive_mutex> lock(container_guard_);
    return (container_.size() == 0);
	}

	template< typename T >
	size_t thread_safe_queue< T >::size() const
	{
    boost::unique_lock<boost::recursive_mutex> lock(container_guard_);
    return container_.size();
	}

}

#endif // _TASK5_6_THREAD_SAFE_QUEUE_H_
