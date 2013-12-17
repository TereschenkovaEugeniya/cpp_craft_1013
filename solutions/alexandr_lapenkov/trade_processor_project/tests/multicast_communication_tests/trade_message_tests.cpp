#include "test_registrator.h"

#include <trade_message.h>

namespace multicast_communication
{
	namespace tests_
	{
		void callback( const trade_message_ptr& msg)
		{
			BOOST_CHECK_EQUAL( msg->get_price(), 779000 );
		}
	}
}

void multicast_communication::tests_::trade_message_tests()
{
	trade_message_ptr tm( new trade_message( &callback ) );
	std::string message;
	std::ifstream in( SOURCE_DIR"/tests/data/trade_messages" );
	std::getline( in, message );
	std::stringstream stream( message.substr( 3 ) );
	binreader reader( stream );
	BOOST_CHECK_NO_THROW( tm->parse( reader, 'E', 'B' ) );
}