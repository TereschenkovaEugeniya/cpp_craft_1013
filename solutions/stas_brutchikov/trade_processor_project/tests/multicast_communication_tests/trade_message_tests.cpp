#include "test_registrator.h"

#include <trade_message.h>

void multicast_communication::tests_::trade_message_tests()
{
	BOOST_CHECK_NO_THROW
	(
		trade_message tm;

		BOOST_CHECK_EQUAL( tm.security_symbol(), "" );
		BOOST_CHECK_EQUAL( tm.price(), 0.0 );
		BOOST_CHECK_EQUAL( tm.volume(), 0.0 );
	)

	BOOST_CHECK_NO_THROW
	(
		std::stringstream ss("ZZZ@0123D00123400DF ");
        trade_message tm(ss,  type_short);

		BOOST_CHECK_EQUAL( tm.security_symbol(), "ZZZ" );
		BOOST_CHECK_EQUAL( tm.price(),  12.34);
		BOOST_CHECK_EQUAL( tm.volume(), 123.0);
    )


	BOOST_CHECK_NO_THROW
	(
		std::stringstream ss("ZZZ  ZZZ        0     000 F  1  D000000123000000000123DD 0");

        trade_message tm(ss,  type_long);		
		BOOST_CHECK_EQUAL( tm.security_symbol(), "ZZZ  ZZZ   " );
		BOOST_CHECK_EQUAL( tm.price(), 12.3 );
		BOOST_CHECK_EQUAL( tm.volume(), 123.0 );
    )

}
