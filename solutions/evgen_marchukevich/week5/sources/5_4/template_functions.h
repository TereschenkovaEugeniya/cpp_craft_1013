#ifndef _TASK5_4_TEMPLATE_FUNCTIONS_H_
#define _TASK5_4_TEMPLATE_FUNCTIONS_H_

#include <functional>
#include <boost/foreach.hpp>

namespace task5_4
{

	template< bool delete_first, typename Container >
	struct ClearContainerOneArg
	{
		void Clean(Container &a)
		{

		}
	};

	template < typename Container >
    struct ClearContainerOneArg< true, Container >
    {
		void Clean(Container &a)
		{
			BOOST_FOREACH(const Container :: value_type &temp, a)
			{
				delete temp;
			}
		}
	};


	template< bool delete_first, bool delete_second, typename Container >
    struct ClearContainerTwoArgs
    {
		void Clean(Container& a)
		{

		}
    };

	template<bool delete_second, typename Container >
    struct ClearContainerTwoArgs<true,delete_second,Container>
    {
		void Clean(Container& a)
		{
			BOOST_FOREACH(const Container :: value_type &temp, a)
			{
				delete temp.first;
			}
	
		}
    };

	template<bool delete_first, typename Container >
    struct ClearContainerTwoArgs<delete_first,true,Container>
    {
		void Clean(Container& a)
		{
			BOOST_FOREACH(const Container :: value_type &temp, a)
			{
				delete temp.second;
			}
	
		}
    };

	template< typename Container >
    struct ClearContainerTwoArgs<true,true,Container>
    {
		void Clean(Container& a)
		{
			BOOST_FOREACH(const Container :: value_type &temp, a)
			{
				delete temp.first;
				delete temp.second;
			}
	
		}
    };



	template< bool delete_first, typename Container >
	void clear_container( Container& a)
	{
		ClearContainerOneArg<delete_first,Container> Clearer;
		Clearer.Clean(a);
		a.clear();	
	}

	template< bool delete_first, bool delete_second, typename Container >
	void clear_container( Container& a)
	{		
		ClearContainerTwoArgs<delete_first,delete_second,Container> Clearer;
		Clearer.Clean(a);
		a.clear();
	}
}


#endif // _TASK5_4_TEMPLATE_FUNCTIONS_H_

