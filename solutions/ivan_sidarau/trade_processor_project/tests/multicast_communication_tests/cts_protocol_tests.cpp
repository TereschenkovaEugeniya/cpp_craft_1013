#include "test_registrator.h"

#include <cts_protocol.h>

void multicast_communication::tests_::cts_protocol_tests()
{
	BOOST_CHECK_EQUAL( sizeof( cts_protocol::message_header ), 24ul );
	BOOST_CHECK_EQUAL( sizeof( cts_protocol::short_trade ), 20ul );
	BOOST_CHECK_EQUAL( sizeof( cts_protocol::long_trade ), 58ul );
}
