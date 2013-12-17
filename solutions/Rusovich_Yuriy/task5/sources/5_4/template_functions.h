#ifndef _TASK5_4_TEMPLATE_FUNCTIONS_H_
#define _TASK5_4_TEMPLATE_FUNCTIONS_H_

#include <functional>

// Unfortunately partial specialization don`t work for funktions((

namespace task5_4
{
	template <typename Container, bool b>
	class ClearContaiter
	{
	private:
		ClearContaiter();
	public:
		static void clearContainer(Container &c)
		{}
	};

	template <typename Container>
	class ClearContaiter<Container, true>
	{
	private:
		ClearContaiter();
	public:
		static void clearContainer(Container &c)
		{
			for(Container::iterator it = c.begin();	it != c.end(); ++it)
				delete *it;
		}
	};
	
	template< bool delete_first, typename Container >
	void clear_container( Container& c)
	{
		ClearContaiter<Container, delete_first>::clearContainer(c);
		c.clear();
	}

	template <typename Container, bool f, bool s>
	class ClearContaiterAssociative
	{
	private:
		ClearContaiterAssociative();
	public:
		static void clearContainer(Container &c)
		{}
	};

	template <typename Container>
	class ClearContaiterAssociative<Container, true, false>
	{
	private:
		ClearContaiterAssociative();
	public:
		static void clearContainer(Container &c)
		{
			for(Container::iterator it = c.begin(); it != c.end(); ++it)
				delete it->first;
		}
	};

	template <typename Container>
	class ClearContaiterAssociative<Container, false, true>
	{
	private:
		ClearContaiterAssociative();
	public:
		static void clearContainer(Container &c)
		{
			for(Container::iterator it = c.begin(); it != c.end(); ++it)
				delete it->second;
		}
	};

	template <typename Container>
	class ClearContaiterAssociative<Container, true, true>
	{
	private:
		ClearContaiterAssociative();
	public:
		static void clearContainer(Container &c)
		{
			for(Container::iterator it = c.begin(); it != c.end(); ++it)
			{
				delete it->first;
				delete it->second;
			}
		}
	};

	template< bool delete_first, bool delete_second, typename Container >
	void clear_container( Container& c)
	{
		ClearContaiterAssociative < Container, delete_first, delete_second > ::clearContainer(c);
		c.clear();
	}
}

#endif // _TASK5_4_TEMPLATE_FUNCTIONS_H_

