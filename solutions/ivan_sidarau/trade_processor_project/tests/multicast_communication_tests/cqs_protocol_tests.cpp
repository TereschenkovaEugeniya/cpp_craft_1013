#include "test_registrator.h"

#include <cqs_protocol.h>

void multicast_communication::tests_::cqs_protocol_tests()
{
	BOOST_CHECK_EQUAL( sizeof( cqs_protocol::message_header ), 24ul );
	BOOST_CHECK_EQUAL( sizeof( cqs_protocol::short_quote ), 34ul );
	BOOST_CHECK_EQUAL( sizeof( cqs_protocol::long_quote ), 78ul );
}
