#include "test_registrator.h"

#include <quote_message.h>

void multicast_communication::tests_::quote_message_tests()
{
	BOOST_CHECK_NO_THROW
	(
		quote_message qm;

		BOOST_CHECK_EQUAL( qm.security_symbol(), "" );
		BOOST_CHECK_EQUAL( qm.bid_price(), 0.0 );
		BOOST_CHECK_EQUAL( qm.bid_volume(), 0.0 );
		BOOST_CHECK_EQUAL( qm.offer_price(), 0.0 );
		BOOST_CHECK_EQUAL( qm.offer_volume(), 0.0 );
	)
    


	BOOST_CHECK_NO_THROW
	(
        std::stringstream in ("ZZZR  B00001234006 B00002345004 12");
		quote_message qm(in, quote_message::type_short);
		
		BOOST_CHECK_EQUAL( qm.security_symbol(), "ZZZ" );
		BOOST_CHECK_EQUAL( qm.bid_price(), 12.34 );
		BOOST_CHECK_EQUAL( qm.bid_volume(), 6.0 );
		BOOST_CHECK_EQUAL( qm.offer_price(), 23.45 );
		BOOST_CHECK_EQUAL( qm.offer_volume(), 4.0 );
    )
    

	BOOST_CHECK_NO_THROW
    (                    
		std::stringstream in("ZZZ  ZZZ        0    AAAR CD0000001234000000125D0000000123000000124     A   02");
        quote_message qm(in, quote_message::type_long);
		BOOST_CHECK_EQUAL( qm.security_symbol(), "ZZZ  ZZZ   " );
		BOOST_CHECK_EQUAL( qm.bid_price(), 12.34 );
		BOOST_CHECK_EQUAL( qm.bid_volume(), 125.0 );
		BOOST_CHECK_EQUAL( qm.offer_price(), 1.23 );
		BOOST_CHECK_EQUAL( qm.offer_volume(), 124.0 );
           
    )

}
