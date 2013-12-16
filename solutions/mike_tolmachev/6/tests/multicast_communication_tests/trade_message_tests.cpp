#include "test_registrator.h"

#include <trade_message.h>

void multicast_communication::tests_::trade_message_tests()
{
	double eps = 0.0000001;

	BOOST_CHECK_NO_THROW
	(
		trade_message tm;

		BOOST_CHECK_EQUAL( tm.security_symbol(), "" );
		BOOST_CHECK_EQUAL( tm.price(), 0.0 );
		BOOST_CHECK_EQUAL( tm.volume(), 0.0 );
	)

	{
		std::string msg("EIAO A  000146235T:3]008ACN@0100B00007790DD ");

		trade_message tm;
		tm.initialize(msg);

		BOOST_CHECK_EQUAL( tm.security_symbol(), "ACN" );
		BOOST_CHECK_EQUAL( fabs( tm.price() -  77.90 ) < eps, true );
		BOOST_CHECK_EQUAL( fabs( tm.volume() -  100.0 ) < eps, true );
	}


	{
		std::string msg("EBAO A  000146234N:3]004ACN             0     000 F  1  D000000779000000000100DD 0");

		trade_message tm;
		tm.initialize(msg);

		BOOST_CHECK_EQUAL( tm.security_symbol(), "ACN        " );
		BOOST_CHECK_EQUAL( fabs( tm.price() -  77.90 ) < eps, true );
		BOOST_CHECK_EQUAL( fabs( tm.volume() -  100.0 ) < eps, true );
	}

}
