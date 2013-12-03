#include "test_registrator.h"

#include <quote_message.h>

namespace multicast_communication
{
	namespace tests_
	{
		void callback( const quote_message_ptr& msg )
		{
			BOOST_CHECK_EQUAL( msg->get_bid_price(), 10484 );
		}
	}
}

void multicast_communication::tests_::quote_message_tests()
{
	quote_message_ptr qm( new quote_message( &callback ) );
	std::string cut_quote = "EO A  004391904Z:J_073BUDR  B00010484003 B00010490003 02EDEO A  004391905Z:J_074BUDR  B00010483006 B00010490003 02""\x03";
	std::stringstream stream( cut_quote );
	binreader in( stream );
	BOOST_CHECK_NO_THROW( qm->parse( in, 'E', 'D' ) );
}
