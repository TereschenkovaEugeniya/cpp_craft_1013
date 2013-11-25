#ifndef _TASK5_6_THREAD_SAFE_QUEUE_H_
#define _TASK5_6_THREAD_SAFE_QUEUE_H_

#include <cstdlib>
#include <boost/thread.hpp>

namespace task5_6
{

	template< typename T >
	class thread_safe_queue
	{
	struct queue_member
		{
			T Data;
			queue_member* NextPtr;
		};
	public:
		explicit thread_safe_queue();
		~thread_safe_queue();

		void push( const T& new_element );
		bool pop( T& result );

		bool empty() const;
		size_t size() const;

	private:
		queue_member* ToPopPtr;
		queue_member* ToPushPtr;
		size_t sz;
		mutable boost::mutex PushPopMtx;
	};

	template< typename T >
	thread_safe_queue< T >::thread_safe_queue()
	{
		sz = 0;
	}

	template< typename T >
	thread_safe_queue< T >::~thread_safe_queue()
	{
		while(sz!=0)
		{
			queue_member* tmpPtr = ToPopPtr->NextPtr;
			delete ToPopPtr;
			ToPopPtr = tmpPtr;
			sz--;
		}

	}

	template< typename T >
	void thread_safe_queue< T >::push( const T&  new_element)
	{
		boost::mutex::scoped_lock safe_push(PushPopMtx);
		if (!sz)
		{
			ToPushPtr = new queue_member;
			ToPushPtr->Data = new_element;
			ToPopPtr = ToPushPtr;
			sz++;
		}
		else
		{
			queue_member* NewPtr;

			NewPtr = new queue_member;
			NewPtr->Data = new_element;
			ToPushPtr->NextPtr = NewPtr;
			ToPushPtr = NewPtr;

			sz++;
		}

	}

	template< typename T >
	bool thread_safe_queue< T >::pop( T& result)
	{
		boost::mutex::scoped_lock safe_pop(PushPopMtx);
		bool out;
		if(sz!=0)
		{
			result = ToPopPtr->Data;
			queue_member* tmpPtr = ToPopPtr->NextPtr;
			delete ToPopPtr;
			ToPopPtr = tmpPtr;
			sz--;
			out = true;
		}
		else
			out = false;

		return out;
	}

	template< typename T >
	bool thread_safe_queue< T >::empty() const
	{
		boost::mutex::scoped_lock safe_empty(PushPopMtx);
		return (sz == 0);
	}

	template< typename T >
	size_t thread_safe_queue< T >::size() const
	{
		boost::mutex::scoped_lock safe_size(PushPopMtx);
		return sz;
	}

}

#endif // _TASK5_6_THREAD_SAFE_QUEUE_H_
