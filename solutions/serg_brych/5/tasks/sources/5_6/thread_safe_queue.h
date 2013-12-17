#ifndef _TASK5_6_THREAD_SAFE_QUEUE_H_
#define _TASK5_6_THREAD_SAFE_QUEUE_H_

#include <cstdlib>
#include <mutex>
#include <deque>



namespace task5_6
{
	template< typename T >
	class thread_safe_queue
	{
		mutable std::mutex mutex_;
		size_t size_;
		std::deque<T> deque_;
	public:
		explicit thread_safe_queue();
		~thread_safe_queue();

		void push( const T& new_element );
		bool pop( T& result );

		bool empty() const;
		size_t size() const;
	};

	template< typename T >
	thread_safe_queue< T >::thread_safe_queue():size_(0)
	{
	}

	template< typename T >
	thread_safe_queue< T >::~thread_safe_queue()
	{
	}

	template< typename T >
	void thread_safe_queue< T >::push( const T& value )
	{
		std::unique_lock<std::mutex> lock_(mutex_);
		deque_.push_back(value);
		size_++;
	}

	template< typename T >
	bool thread_safe_queue< T >::pop( T& value)
	{
		std::unique_lock<std::mutex> lock_(mutex_);
		if(size_ == 0)
			return false;
		value = deque_.front();
		deque_.pop_front();
		size_--;
		return true;
	}

	template< typename T >
	bool thread_safe_queue< T >::empty() const
	{
		std::unique_lock<std::mutex> lock_(mutex_);
		return !size_;
	}

	template< typename T >
	size_t thread_safe_queue< T >::size() const
	{
		std::unique_lock<std::mutex> lock_(mutex_);
		return size_;
	}

}

#endif // _TASK5_6_THREAD_SAFE_QUEUE_H_
