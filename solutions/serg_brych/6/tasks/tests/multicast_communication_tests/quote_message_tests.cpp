#include "test_registrator.h"

#include <quote_message.h>

void multicast_communication::tests_::quote_message_tests()
{
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
	}

	// test short quote
	{
		std::stringstream ss("EDEO A  003759032T:J_073ADMR  B00004147006 B00004148004 12");
		quote_message qm;
		BOOST_CHECK_NO_THROW
		(
			qm.parse_quote(ss);
		)
		BOOST_CHECK_EQUAL( qm.security_symbol(), "ADM" );
		BOOST_CHECK_EQUAL( qm.bid_price(), 41.47 );
		BOOST_CHECK_EQUAL( qm.bid_volume(), 6.0 );
		BOOST_CHECK_EQUAL( qm.offer_price(), 41.48 );
		BOOST_CHECK_EQUAL( qm.offer_volume(), 4.0 );
	}

	// test long quote
	{
		std::stringstream ss("EBEO A  003759035N:J_077ALU             0    AAAR CD0000000423000000147D0000000424000000380     A   02");
		quote_message qm;

		BOOST_CHECK_NO_THROW(
			qm.parse_quote(ss);
		)	
		BOOST_CHECK_EQUAL( qm.security_symbol(), "ALU        " );
		BOOST_CHECK_EQUAL( qm.bid_price(), 4.23 );
		BOOST_CHECK_EQUAL( qm.bid_volume(), 147.0 );
		BOOST_CHECK_EQUAL( qm.offer_price(), 4.24 );
		BOOST_CHECK_EQUAL( qm.offer_volume(), 380.0 );
		
	}

	//test short qoute len = 86
	{
		std::stringstream ss("EDEO A  003759033T:J_073ADMR  B00004147001 B00004148004 62KB00004147005 TB00004148004 ");
		quote_message qm;
		BOOST_CHECK_NO_THROW
		(
			qm.parse_quote(ss);
		)
		BOOST_CHECK_EQUAL( qm.security_symbol(), "ADM" );
		BOOST_CHECK_EQUAL( qm.bid_price(), 41.47 );
		BOOST_CHECK_EQUAL( qm.bid_volume(), 1.0 );
		BOOST_CHECK_EQUAL( qm.offer_price(), 41.48 );
		BOOST_CHECK_EQUAL( qm.offer_volume(), 4.0 );
	}


	// test long qoute block different len
	{
		std::stringstream ss("\x01""EBEO A  003759861N:J_872AUO             0    AAAR CD0000000296000000167D0000000297000000043     A   62ND00029600167 PF02970000055"
							 "\x1f""EBEO A  003759862N:J_872ALU             0    AAAR CD0000000423000000151D0000000424000000353     A   02""\x03" );
		quote_message_list_ptr processed_messages;
		quote_message_list_ptr::iterator it;
		quote_message_ptr quote_message_ptr;
		if(quote_message::parse_block(ss.str(), processed_messages))
		{
			it = processed_messages.begin();
			quote_message_ptr = *it;
			BOOST_CHECK_EQUAL( quote_message_ptr->security_symbol(), "AUO        " );
			BOOST_CHECK_EQUAL( quote_message_ptr->bid_price(), 2.96 );
			BOOST_CHECK_EQUAL( quote_message_ptr->bid_volume(), 167.0 );
			BOOST_CHECK_EQUAL( quote_message_ptr->offer_price(), 2.97 );
			BOOST_CHECK_EQUAL( quote_message_ptr->offer_volume(), 43.0 );

			++it;
			quote_message_ptr = *it;
			
			BOOST_CHECK_EQUAL( quote_message_ptr->security_symbol(), "ALU        " );
			BOOST_CHECK_EQUAL( quote_message_ptr->bid_price(), 4.23 );
			BOOST_CHECK_EQUAL( quote_message_ptr->bid_volume(), 151.0 );
			BOOST_CHECK_EQUAL( quote_message_ptr->offer_price(), 4.24 );
			BOOST_CHECK_EQUAL( quote_message_ptr->offer_volume(), 353 );
		}

	}
	
}
