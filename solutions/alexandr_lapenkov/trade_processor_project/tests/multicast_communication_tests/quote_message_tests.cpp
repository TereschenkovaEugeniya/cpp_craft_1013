#include "test_registrator.h"

#include <quote_message.h>

namespace multicast_communication
{
	namespace tests_
	{
		void callback( const quote_message_ptr& msg )
		{
			BOOST_CHECK_EQUAL( msg->get_security_symbol(), "ADM" );
		}
	}
}

void multicast_communication::tests_::quote_message_tests()
{
	quote_message_ptr qm( new quote_message( &callback ) );
	std::string message;
	std::ifstream in( SOURCE_DIR"/tests/data/quote_messages" );
	std::getline( in, message );
	std::stringstream stream( message.substr( 3 ) );
	binreader reader( stream );
	BOOST_CHECK_NO_THROW( qm->parse( reader, 'E', 'D' ) );
}
