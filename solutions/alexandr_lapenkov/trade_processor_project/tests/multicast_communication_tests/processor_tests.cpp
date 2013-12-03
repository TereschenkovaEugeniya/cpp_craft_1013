#include "test_registrator.h"

#include <processor.h>

void multicast_communication::tests_::processor_tests()
{
	{
		std::string message = "\x01""EDEO A  004391904Z:J_073BUDR  B00010484003 B00010490003 02EDEO A  004391905Z:J_074BUDR  B00010483006 B00010490003 02""\x03";
		processor p( "market_data.dat" );
		BOOST_CHECK_NO_THROW( p.parse( message, QUOTE ) )
		message = "EDEO A  004391904Z:J_073BUDR  B00010484003 B00010490003 02EDEO A  004391905Z:J_074BUDR  B00010483006 B00010490003 02""\x03";
		BOOST_CHECK_THROW( p.parse( message, QUOTE ), std::logic_error );
		message = "\x01""EDEO A  004391904Z:J_073BUDR  B00010484003 B00010490003 02EDEO A  004391905Z:J_074BUDR  B00010483006 B00010490003 02";
		BOOST_CHECK_THROW( p.parse( message, QUOTE ), std::logic_error );
		message = "\x01""EBAO A  000146234N:3]004ACN             0     000 F  1  D000000779000000000100DD 0""\x03";
		BOOST_CHECK_NO_THROW( p.parse( message, TRADE ) );
		message = "wrong trade";
		BOOST_CHECK_THROW( p.parse( message, TRADE ), std::logic_error );
	}
}
