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
	std::string cut_trade = "AO A  000146234N:3]004ACN             0     000 F  1  D000000779000000000100DD 0";
	std::stringstream stream( cut_trade );
	binreader in( stream );
	BOOST_CHECK_NO_THROW( tm->parse( in, 'E', 'B' ) );
}