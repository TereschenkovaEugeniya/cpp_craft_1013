#ifndef _TASK5_6_THREAD_SAFE_QUEUE_H_
#define _TASK5_6_THREAD_SAFE_QUEUE_H_

#include <cstdlib>
#include <boost/thread.hpp>

namespace task5_6
{
	template< typename T >
	class thread_safe_queue
	{
	private :
		struct Node_
		{
			T val_;
			Node_* next_;

			Node_()
			{
				val_ = T();
				next_ = NULL;
			}
		};

		mutable boost::mutex queue_;
		mutable boost::mutex empty_;

		Node_* begin_;
		Node_* end_;

	public :
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
		begin_ = new Node_();
		end_ = begin_;
	}

	template< typename T >
	thread_safe_queue< T >::~thread_safe_queue()
	{
		Node_* cur = begin_;
		
		while ( cur != end_ )
		{
			begin_ = begin_->next_;
			delete cur;
			cur = begin_;
		}

		delete cur;
	}

	template< typename T >
	void thread_safe_queue< T >::push( const T& el )
	{
		boost::mutex::scoped_lock protect_queue( queue_ );

		std::memcpy( &end_->val_, &el, sizeof(T) );

		end_->next_ = new Node_();
		end_ = end_->next_;
	}

	template< typename T >
	bool thread_safe_queue< T >::pop( T& poped_el )
	{

		boost::mutex::scoped_lock protect_queue( queue_ );
		if ( begin_ == end_ )
		{
			return false;
		}
		else 
		{
			std::memcpy( &poped_el, &(begin_->val_), sizeof(T) );
			Node_* poped = begin_;
			begin_ = begin_->next_;

			delete poped;
			return true;
		}
	}

	template< typename T >
	bool thread_safe_queue< T >::empty() const
	{
		boost::mutex::scoped_lock protect_queue( queue_ );
		return begin_ == end_;
	}

	template< typename T >
	size_t thread_safe_queue< T >::size() const
	{
		boost::mutex::scoped_lock protect_queue( queue_ );
		Node_* tracker = begin_;
		
		size_t size = 0;

		while( tracker->next_ )
		{
			size++;
			tracker = tracker->next_;
		}

		return size;
	}

}

#endif // _TASK5_6_THREAD_SAFE_QUEUE_H_
