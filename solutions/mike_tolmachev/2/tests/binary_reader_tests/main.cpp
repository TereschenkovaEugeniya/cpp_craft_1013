#include <reader.h>

#include <iostream>
#include <fstream>

#define test_no_throw( action ) \
{ \
	try \
	{ \
		action; \
	} \
	catch( ... ) \
	{ \
		std::cout << "test_no_throw failed: " <<  __FILE__ << ":" << __LINE__ << std::endl; \
	} \
} \



int main()
{
	test_no_throw( binary_reader::proxy_function_that_was_created_only_for_compiling_goals() );
}
