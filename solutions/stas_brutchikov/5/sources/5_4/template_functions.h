#ifndef _TASK5_4_TEMPLATE_FUNCTIONS_H_
#define _TASK5_4_TEMPLATE_FUNCTIONS_H_

#include <functional>
#include <algorithm>

namespace task5_4
{
    template< typename Container, bool delete_first >
    struct ClearContainerFirst
    {
        static void clear_container( Container& container )
        {
        }
    };


    template< typename Container >
    struct ClearContainerFirst<Container, true>
    {
        static void clear_container( Container& container )
        {
            std::for_each( container.begin(), container.end(), [](Container::value_type it){delete it;});
        }
    };


	template< bool delete_first, typename Container >
	void clear_container( Container& container )
	{
        ClearContainerFirst<Container, delete_first>::clear_container(container);
        container.clear();
	}


    template< typename Container, bool delete_first, bool delete_second >
    struct ClearContainerFirstSecond
    {
        static void clear_container( Container& container )
        {
        }
    };


    template< typename Container >
    struct ClearContainerFirstSecond<Container, false, true>
    {
        static void clear_container( Container& container )
        {
            std::for_each( container.begin(), container.end(), [](Container::value_type it){delete it.second;});
        }
    };


    template< typename Container >
    struct ClearContainerFirstSecond<Container, true, false>
    {
        static void clear_container( Container& container )
        {
            std::for_each( container.begin(), container.end(), [](Container::value_type it){delete it.first;});
        }
    };


    template< typename Container>
    struct ClearContainerFirstSecond<Container, true, true>
    {
        static void clear_container( Container& container )
        {
            std::for_each( container.begin(), container.end(), [](Container::value_type it){delete it.first; delete it.second;});
        }
    };


	template< bool delete_first, bool delete_second, typename Container >
	void clear_container( Container& container )
	{
        ClearContainerFirstSecond<Container, delete_first, delete_second>::clear_container(container);
        container.clear();

	}
}


#endif // _TASK5_4_TEMPLATE_FUNCTIONS_H_

