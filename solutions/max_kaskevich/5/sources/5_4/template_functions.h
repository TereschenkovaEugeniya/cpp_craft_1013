#ifndef _TASK5_4_TEMPLATE_FUNCTIONS_H_
#define _TASK5_4_TEMPLATE_FUNCTIONS_H_

#include <functional>

namespace task5_4
{
    // non-associative containers

    template< typename Container, bool delete_first >
    struct clear_container_class
    {
        static void clear_container( Container& container )
        {
        }
    };

    template<  typename Container>
    struct clear_container_class< Container, true >
    {
        static void clear_container( Container& container )
        {
            for( auto& elem: container )
            {
                delete elem;
            }
        }
    };


	template< bool delete_first, typename Container >
	void clear_container( Container& container)
    {
        clear_container_class< Container, delete_first >::clear_container( container );
        container.clear();
    }

    // associative containers 

    template< typename Container, bool delete_first, bool delete_second >
    struct clear_assoc_container_class
    {
        static void clear_container( Container& container )
        {
        }
    };

    template<  typename Container>
    struct clear_assoc_container_class< Container, true, false >
    {
        static void clear_container( Container& container )
        {
            for( auto& pair: container )
            {
                delete pair.first;
            }
        }
    };

    template<  typename Container>
    struct clear_assoc_container_class< Container, false, true >
    {
        static void clear_container( Container& container )
        {
            for( auto& pair: container )
            {
                delete pair.second;
            }
        }
    };

    template<  typename Container>
    struct clear_assoc_container_class< Container, true, true >
    {
        static void clear_container( Container& container )
        {
            for( auto& pair: container )
            {
                delete pair.first;
                delete pair.second;
            }
        }
    };

	template< bool delete_first, bool delete_second, typename Container >
	void clear_container( Container& container )
	{
        clear_assoc_container_class< Container, delete_first, delete_second >::
            clear_container( container );
        container.clear();
	}
}


#endif // _TASK5_4_TEMPLATE_FUNCTIONS_H_

