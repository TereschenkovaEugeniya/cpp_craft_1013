#ifndef _TASK5_4_TEMPLATE_FUNCTIONS_H_
#define _TASK5_4_TEMPLATE_FUNCTIONS_H_

#include <functional>

namespace task5_4
{
	/*template< bool delete_first, typename Container > //somehow not working: 4 errors cannot use delete
	void clear_container( Container& c, bool b = delete_first)
	{
		for(typename iter it = c.begin(); it != c.end(); )
		{
			if(delete_first) 
			{
				delete *it;
				it = c.erase (it) ;
			}
			else
			{
				it = c.erase (it);
			}
		}
		}*/
	template<bool delete_first, typename Container> void clear_container(Container & c, char(*)[delete_first] = 0) 
	{
		for(typename Container::iterator it = c.begin(); it != c.end(); it++)
		{
			delete *it;
		}
		c.clear();
	}
	template<bool delete_first, typename Container> void clear_container(Container & c, char(*)[!delete_first] = 0) 
	{
		c.clear();
	}
	template<bool delete_first, bool delete_second, typename Container> void clear_container(Container & c, char(*)[delete_first] = 0,
		char(*)[delete_second] = 0) 
	{
		for(typename Container::iterator it = c.begin(); it != c.end(); it++)
		{
			delete it->first;
			delete it->second;
		}
		c.clear();
	}
	template<bool delete_first, bool delete_second, typename Container> void clear_container(Container & c, char(*)[delete_first] = 0,
		char(*)[!delete_second] = 0) 
	{
		for(typename Container::iterator it = c.begin(); it != c.end(); it++)
		{
			delete it->first;
		}
		c.clear();
	}
	template<bool delete_first, bool delete_second, typename Container> void clear_container(Container & c, char(*)[!delete_first ] = 0,
		char(*)[delete_second] = 0) 
	{
		for(typename Container::iterator it = c.begin(); it != c.end(); it++)
		{
			delete it->second;
		}
		c.clear();
	}
	template<bool delete_first, bool delete_second, typename Container> void clear_container(Container & c, char(*)[!delete_first ] = 0,
		char(*)[!delete_second ] = 0) 
	{
		c.clear () ;
	}
/*	template< bool delete_first, bool delete_second, typename Container >
	void clear_container( Container& )
	{
	}*/
}


#endif // _TASK5_4_TEMPLATE_FUNCTIONS_H_

