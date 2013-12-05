#ifndef _TASK5_4_TEMPLATE_FUNCTIONS_H_
#define _TASK5_4_TEMPLATE_FUNCTIONS_H_

#include <functional>
#include <iostream>
#include <boost/utility/enable_if.hpp>

namespace task5_4
{

    template<bool delete_first, typename Container >
        struct ClearContainerHelper
        {
            static void clear_container( Container& ){};
        };

    template <typename Container >
        struct ClearContainerHelper<true, Container>
        {
            static void clear_container( Container& t)
            {
                for(typename Container::iterator it = t.begin(); it != t.end(); ++it)
                {
                    delete *it; 
                }
            }
        }; 

    template< bool delete_first, typename Container >
        void clear_container( Container& t)
        {
            ClearContainerHelper<delete_first, Container>::clear_container(t);
            t.clear();
        } 

    template <bool delete_first, bool delete_second, typename Container>
        struct ClearContainerHelper2
        {
            static void clear_container ( Container& ) {}
        };
    template <bool delete_second, typename Container>
        struct ClearContainerHelper2<true, delete_second, Container>
        {
            static void clear_container ( Container& t) {
                for(typename Container::iterator it = t.begin(); it != t.end(); ++it)
                {
                    delete (*it).first; 
                }
            }
        };

    template <bool delete_first, typename Container>
        struct ClearContainerHelper2<delete_first, true, Container>
        {
            static void clear_container ( Container& t) {
                for(typename Container::iterator it = t.begin(); it != t.end(); ++it)
                {
                    delete (*it).second; 
                }
            }
        };

    template <typename Container>
        struct ClearContainerHelper2<true, true, Container>
        {
            static void clear_container ( Container& t) {
                for(typename Container::iterator it = t.begin(); it != t.end(); ++it)
                {
                    delete (*it).first; 
                    delete (*it).second; 
                }
            }
        };

    template< bool delete_first, bool delete_second, typename Container >
        void clear_container( Container& t)
        {
            ClearContainerHelper2<delete_first, delete_second, Container>::clear_container(t);
            t.clear();
        }
} 

#endif // _TASK5_4_TEMPLATE_FUNCTIONS_H_ 
