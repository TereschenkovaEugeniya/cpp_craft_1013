#include "test_registrator.h"

#include <ts_minute_storage.h>

#include <fstream>

#include <boost/filesystem.hpp>

void minute_calculator::tests_::ts_minute_storage_tests()
{
	using multicast_communication::trade_message;
	using multicast_communication::quote_message;

	BOOST_CHECK_NO_THROW( ts_minute_storage ms; );

	{
		ts_minute_storage ms;
		BOOST_CHECK_EQUAL( ms.new_data( 
			trade_message::create_test_message( 
				common::create_uint_time( 14, 05, 23 , 200 ), "A", 14.5, 15.6 ) ).get() == NULL, true );
		BOOST_CHECK_EQUAL( ms.new_data( 
			trade_message::create_test_message( 
				common::create_uint_time( 14, 05, 23 , 010 ), "A", 14.7, 15.6 ) ).get() == NULL, true );
		BOOST_CHECK_EQUAL( ms.new_data( 
			trade_message::create_test_message( 
				common::create_uint_time( 14, 05, 23 , 710 ), "A", 14.9, 15.6 ) ).get() == NULL, true );
		BOOST_CHECK_EQUAL( ms.new_data( 
			trade_message::create_test_message( 
				common::create_uint_time( 14, 05, 23 , 610 ), "A", 14.13, 15.6 ) ).get() == NULL, true );

		stock_minute_data_ptr ptr = ms.new_data( 
			trade_message::create_test_message( 
				common::create_uint_time( 14, 06, 01 , 001 ), "A", 14.14, 15.6 ) );
		BOOST_CHECK_EQUAL( ptr.get() != NULL, true );

		ms.get_last_minute();
	}
}
