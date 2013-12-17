#ifndef _TASK5_4_TEMPLATE_FUNCTIONS_H_
#define _TASK5_4_TEMPLATE_FUNCTIONS_H_

#include <functional>
#include <boost/foreach.hpp>

namespace task5_4
{
	template < bool delete_first, typename Container >
	struct cleaner
	{
		void clean( Container& a )
		{}
	};

	template < typename Container >
	struct cleaner < true, Container >
	{
		void clean( Container& a )
		{
			BOOST_FOREACH( const Container::value_type& var, a )
				delete var;
		}
	};

	template< bool delete_first, bool delete_second, typename Container >
	struct cleaner2args
	{
		void clean( Container& a )
		{}
	};

	template< bool delete_first, typename Container >
	struct cleaner2args < delete_first, true, Container >
	{
		void clean( Container& a )
		{
			BOOST_FOREACH( const Container::value_type& var, a )
				delete var.second;
		}
	};

	template< bool delete_second, typename Container >
	struct cleaner2args < true, delete_second, Container >
	{
		void clean( Container& a )
		{
			BOOST_FOREACH( const Container::value_type& var, a )
				delete var.first;
		}
	};

	template< typename Container >
	struct cleaner2args < true, true, Container >
	{
		void clean( Container& a )
		{
			BOOST_FOREACH( const Container::value_type& var, a )
			{
				delete var.first;
				delete var.second;
			}
		}
	};
	
	template< bool delete_first, typename Container >
	void clear_container( Container& a )
	{
		cleaner< delete_first, Container > cleaner;

		cleaner.clean( a );

		a.clear();
	}

	template< bool delete_first, bool delete_second, typename Container >
	void clear_container( Container& a )
	{
		cleaner2args< delete_first, delete_second, Container > cleaner;

		cleaner.clean( a );

		a.clear();
	}
}


#endif // _TASK5_4_TEMPLATE_FUNCTIONS_H_

