#ifndef _TASK5_6_THREAD_SAFE_QUEUE_H_
#define _TASK5_6_THREAD_SAFE_QUEUE_H_

#include <cstdlib>
#include <boost\thread.hpp>

namespace task5_6
{
	template< typename T >
	class thread_safe_queue
	{
		struct node
		{
			T data_;

			std::shared_ptr<node> next_;
		};
	public:
		explicit thread_safe_queue();
		~thread_safe_queue();

		void push( const T& new_element );
		bool pop( T& result );

		bool empty() const;
		size_t size() const;

	private:
		std::shared_ptr<node> head_;
		std::shared_ptr<node> tail_;

		size_t size_;

		mutable boost::shared_mutex mtx_;
	};

	template< typename T >
	thread_safe_queue< T >::thread_safe_queue() : size_(0), head_(NULL), tail_(NULL)
	{
	}

	template< typename T >
	thread_safe_queue< T >::~thread_safe_queue()
	{
	}

	template< typename T >
	void thread_safe_queue< T >::push( const T& data )
	{
		boost::unique_lock<boost::shared_mutex> lock(mtx_);

		std::shared_ptr<node> new_node(new node);
        new_node->data_ = data;
		
		if (tail_ == NULL && head_ == NULL)
		{
			head_ = new_node;			
		}
		else
		{
			tail_->next_ = new_node;
		}
		tail_ = new_node;

		++size_;
	}

	template< typename T >
	bool thread_safe_queue< T >::pop( T& data )
	{
		boost::unique_lock<boost::shared_mutex> lock(mtx_);

		bool res = false;
		if (head_)
		{
			data = head_->data_;			
			head_ = head_->next_;

			if (head_ == NULL)
			{
				tail_.reset();
			}
						
			--size_;
			res = true;
		}

		return res;
	}

	template< typename T >
	bool thread_safe_queue< T >::empty() const
	{
		boost::shared_lock<boost::shared_mutex> lock(mtx_);

		return (size_ == 0);
	}

	template< typename T >
	size_t thread_safe_queue< T >::size() const
	{
		boost::shared_lock<boost::shared_mutex> lock(mtx_);

		return size_;
	}

}

#endif // _TASK5_6_THREAD_SAFE_QUEUE_H_
