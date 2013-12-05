#ifndef _TASK5_4_TEMPLATE_FUNCTIONS_H_
#define _TASK5_4_TEMPLATE_FUNCTIONS_H_

#include <functional>

namespace task5_4
{
	template< bool delete_first, typename Container >
	void clear_container( Container& c )
	{
		MonoHelper< delete_first, Container >::clear_container( c );
	}

	template< bool delete_first, typename Container >
	struct MonoHelper 
	{
		static void clear_container( Container& c )
		{
			c.clear();
		}
	};

	template< typename Container >
	struct MonoHelper< true, Container >
	{
		static void clear_container( Container& c )
		{
			for ( typename Container::iterator i = c.begin(); i != c.end(); i++ )
			{
				delete *i;
			}

			c.clear();
		}
	};

	template< bool delete_first, bool delete_second, typename Container >
	void clear_container( Container& c )
	{
		BiHelper< delete_first, delete_second, Container >::clear_container( c );
	}

	template< bool delete_first, bool delete_second, typename Container >
	struct BiHelper
	{
		static void clear_container( Container& c )
		{
			c.clear();
		}
	};

	template< typename Container >
	struct BiHelper< true, true, Container >
	{
		static void clear_container( Container& c )
		{
			for ( typename Container::iterator i = c.begin(); i != c.end(); i++ )
			{
				delete i->first;
				delete i->second;
			}
			c.clear();
		}
	};

	template< bool delete_second, typename Container >
	struct BiHelper< true, delete_second, Container >
	{
		static void clear_container( Container& c )
		{
			for ( typename Container::iterator i = c.begin(); i != c.end(); i++ )
			{
				delete i->first;
			}
			c.clear();
		}
	};

	template< bool delete_first, typename Container >
	struct BiHelper< delete_first, true, Container >
	{
		static void clear_container( Container& c )
		{
			for ( typename Container::iterator i = c.begin(); i != c.end(); i++ )
			{
				delete i->second;
			}
			c.clear();
		}
	};
}


#endif // _TASK5_4_TEMPLATE_FUNCTIONS_H_

