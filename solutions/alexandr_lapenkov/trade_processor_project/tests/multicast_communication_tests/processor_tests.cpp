#include "test_registrator.h"

#include <processor.h>

void multicast_communication::tests_::processor_tests()
{
	{
		std::string message;
		std::ifstream in( SOURCE_DIR"/tests/data/quote_messages" );
		std::getline( in, message );
		processor p( "market_data.dat" );
		BOOST_CHECK_NO_THROW( p.parse( message, QUOTE ) )
		BOOST_CHECK_NO_THROW( p.parse( message.substr( 1 ), QUOTE ) );
		BOOST_CHECK_NO_THROW( p.parse( message.substr( 0, message.length() - 1 ), QUOTE ) );
		in.close();
		in.open( SOURCE_DIR"/tests/data/trade_messages", std::ios_base::in );
		BOOST_CHECK_NO_THROW( p.parse( message, TRADE ) );
		message = "wrong trade";
		BOOST_CHECK_NO_THROW( p.parse( message, TRADE ) );
	}
}
