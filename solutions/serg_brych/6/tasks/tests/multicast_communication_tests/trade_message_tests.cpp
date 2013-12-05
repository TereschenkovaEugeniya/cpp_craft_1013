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

	// test short trade
	{
		std::stringstream ss("EIAO A  000140208P:3\\806BGS@0100D00330400DF ");
		trade_message tm;
		BOOST_CHECK_NO_THROW
		(
			tm.parse_trade(ss);
		)
		BOOST_CHECK_EQUAL( tm.security_symbol(), "BGS" );
		BOOST_CHECK_EQUAL( tm.price(),  33.04);
		BOOST_CHECK_EQUAL( tm.volume(), 100.0);
	}


	// test long trade
	{
		std::stringstream ss("EBAO A  000146234N:3]004ACN             0     000 F  1  D000000779000000000100DD 0");
		trade_message tm;
		BOOST_CHECK_NO_THROW
		(
			tm.parse_trade(ss);
		)
		BOOST_CHECK_EQUAL( tm.security_symbol(), "ACN        " );
		BOOST_CHECK_EQUAL( tm.price(), 77.9 );
		BOOST_CHECK_EQUAL( tm.volume(), 100.0 );
	}

	// test long trade block
	{
		std::stringstream ss("\x01""EBAO A  000173807D:3\\839XRX          T  0     000@   0  B000000001102000003400DD 0"
							 "\x1f""EBAO A  000173808T:3\\880VZ              0     000 F  1  B000000005030000000100DD 0""\x03");
	
		trade_message_list_ptr processed_messages;
		trade_message_list_ptr::iterator it;
		trade_message_ptr trade_message_ptr;
		if(trade_message::parse_block(ss.str(), processed_messages))
		{
			it = processed_messages.begin();
			trade_message_ptr = *it;
			BOOST_CHECK_EQUAL( trade_message_ptr->security_symbol(), "XRX        " );
			BOOST_CHECK_EQUAL( trade_message_ptr->price(), 11.02 );
			BOOST_CHECK_EQUAL( trade_message_ptr->volume(), 3400.0 );

			++it;
			trade_message_ptr = *it;
			
			BOOST_CHECK_EQUAL( trade_message_ptr->security_symbol(), "VZ         " );
			BOOST_CHECK_EQUAL( trade_message_ptr->price(), 50.30 );
			BOOST_CHECK_EQUAL( trade_message_ptr->volume(), 100.0 );
		}
	}


}
