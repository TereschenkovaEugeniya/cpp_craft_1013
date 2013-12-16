#include "test_registrator.h"

#include <quote_message.h>

void multicast_communication::tests_::quote_message_tests()
{
	double eps = 0.0000001;

	BOOST_CHECK_NO_THROW
	(
		quote_message qm;

		BOOST_CHECK_EQUAL( qm.security_symbol(), "" );
		BOOST_CHECK_EQUAL( qm.bid_price(), 0.0 );
		BOOST_CHECK_EQUAL( qm.bid_volume(), 0.0 );
		BOOST_CHECK_EQUAL( qm.offer_price(), 0.0 );
		BOOST_CHECK_EQUAL( qm.offer_volume(), 0.0 );
	)

	{
		std::string msg("EDEO A  003759032T:J_073ADMR  B00004147006 B00004148004 12");

		quote_message qm;
		qm.initialize(msg);

		BOOST_CHECK_EQUAL( qm.security_symbol(), "ADM" );
		BOOST_CHECK_EQUAL( fabs( qm.bid_price() -  41.47 ) < eps, true );
		BOOST_CHECK_EQUAL( fabs( qm.bid_volume() -  6.0 ) < eps, true );
		BOOST_CHECK_EQUAL( fabs( qm.offer_price() -  41.48 ) < eps, true );
		BOOST_CHECK_EQUAL( fabs( qm.offer_volume() -  4.0 ) < eps, true );
	}

	{
		std::string msg("EBEO A  003759692Y:J_839AVB             0    AAAR BB0000000121290000001B0000000121580000001     A   02");

		quote_message qm;
		qm.initialize(msg);

		BOOST_CHECK_EQUAL( qm.security_symbol(), "AVB        " );
		BOOST_CHECK_EQUAL( fabs( qm.bid_price() -  121.29 ) < eps, true );
		BOOST_CHECK_EQUAL( fabs( qm.bid_volume() -  1.0 ) < eps, true );
		BOOST_CHECK_EQUAL( fabs( qm.offer_price() -  121.58 ) < eps, true );
		BOOST_CHECK_EQUAL( fabs( qm.offer_volume() -  1.0 ) < eps, true );
	}
}
