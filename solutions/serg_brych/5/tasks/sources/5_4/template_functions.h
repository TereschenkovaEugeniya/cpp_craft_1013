#ifndef _TASK5_4_TEMPLATE_FUNCTIONS_H_
#define _TASK5_4_TEMPLATE_FUNCTIONS_H_

#include <functional>

namespace task5_4
{
	template <typename Container, bool delete_first>
	struct CleanContainer
	{
		static void clear_container(Container &c)
		{
		}
	};

	template <typename Container>
	struct CleanContainer<Container, true>
	{
		static void clear_container(Container &c)
		{
			for(auto &i: c)
			{
				delete i;
			}
		}
	};

	template< bool delete_first, typename Container >
	void clear_container( Container& c)
	{
		CleanContainer<Container, delete_first>::clear_container(c);
		c.clear();
	}
	
	template<typename Container,  bool delete_first, bool delete_second>
	struct CleanContainerEx
	{
		static void clear_container(Container &c)
		{
		
		}
	};

	template<typename Container>
	struct CleanContainerEx<Container, true, true>
	{
		static void clear_container(Container &c)
		{
			for(auto &i: c)
			{
				delete i.first;
				delete i.second;
			}
		}
	};

	template<typename Container>
	struct CleanContainerEx<Container, false, true>
	{
		static void clear_container(Container &c)
		{
			for(auto &i: c)
			{
				delete i.second;
			}
		}
	};

	template<typename Container>
	struct CleanContainerEx<Container, true, false>
	{
		static void clear_container(Container &c)
		{
			for(auto &i: c)
			{
				delete i.first;
			}
		}
	};
	
	
	template< bool delete_first, bool delete_second, typename Container >
	void clear_container( Container& c)
	{
		CleanContainerEx<Container, delete_first, delete_second>::clear_container(c);
		c.clear();
	}
}


#endif // _TASK5_4_TEMPLATE_FUNCTIONS_H_

