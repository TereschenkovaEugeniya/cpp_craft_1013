#include "test_registrator.h"

#include <address.h>

void common::tests_::address_tests()
{
	BOOST_CHECK_NO_THROW
	(
		address a1( "0.0.0.0", 61128 );
		address a2( "255.255.0.0", 50000 );
		address a3( "10.0.0.2", 80 );

		BOOST_CHECK_EQUAL( a1.endpoint().address().to_string(), "0.0.0.0" );
		BOOST_CHECK_EQUAL( a1.endpoint().port(), 61128u );
		BOOST_CHECK_EQUAL( a2.endpoint().address().to_string(), "255.255.0.0" );
		BOOST_CHECK_EQUAL( a2.endpoint().port(), 50000u );
		BOOST_CHECK_EQUAL( a3.endpoint().address().to_string(), "10.0.0.2" );
		BOOST_CHECK_EQUAL( a3.endpoint().port(), 80u );

		BOOST_CHECK_EQUAL( a1.ip(), "0.0.0.0" );
		BOOST_CHECK_EQUAL( a2.ip(), "255.255.0.0" );
		BOOST_CHECK_EQUAL( a3.ip(), "10.0.0.2" );

		BOOST_CHECK_EQUAL( a1.port(), 61128u );
		BOOST_CHECK_EQUAL( a2.port(), 50000u );
		BOOST_CHECK_EQUAL( a3.port(), 80u );
	);
}
