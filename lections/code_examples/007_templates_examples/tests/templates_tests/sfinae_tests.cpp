#include "test_registrator.h"

#include <sfinae.h>

void templates::tests_::sfinae_tests()
{
	BOOST_CHECK_EQUAL( inheritance_test< A1 >::check, true );
	BOOST_CHECK_EQUAL( inheritance_test< B1 >::check, false );
}
