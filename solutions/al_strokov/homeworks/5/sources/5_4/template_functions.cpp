#include "template_functions.h"

#include <vector>
#include <map>

namespace task5_4
{
	void instantiation_test();
}
void task5_4::instantiation_test()
{
	std::vector< int > v1;
	clear_container( v1 );

	std::vector< int* > v2;
	clear_container( v2 );

// uncomment to see compilation problem
//		std::vector< int > v3;
//		clear_container< false, true >( v3 );

	std::map< int, int > m1;
	clear_container( m1 );

	std::map< int, int* > m2;
	clear_container( m2 );

	std::map< int*, int > m3;
	clear_container( m3 );

	std::map< int*, int* > m4;
	clear_container( m4 );
}	
