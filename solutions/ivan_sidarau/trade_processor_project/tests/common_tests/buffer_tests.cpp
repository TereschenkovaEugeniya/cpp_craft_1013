#include "test_registrator.h"

#include <buffer.h>

void common::tests_::buffer_tests()
{
	BOOST_CHECK_NO_THROW
	(
		buffer_ptr buffer_test( new buffer() );
		BOOST_CHECK_EQUAL( buffer::size, 1024ul );
	);
}
