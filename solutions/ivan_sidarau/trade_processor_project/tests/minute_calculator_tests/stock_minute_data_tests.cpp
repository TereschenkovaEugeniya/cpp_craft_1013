#include "test_registrator.h"

#include <stock_minute_data.h>

#include <fstream>

#include <boost/filesystem.hpp>

void minute_calculator::tests_::stock_minute_data_tests()
{
	using multicast_communication::trade_message;
	using multicast_communication::quote_message;

	BOOST_CHECK_NO_THROW( stock_minute_data smd( trade_message::create_test_message( 12u, "ASD", 10.2, 15.3 ) ); );
	{
		stock_minute_data smd( trade_message::create_test_message( 12u, "ASD", 10.2, 15.3 ) );
		std::stringstream test;
		test << smd;
		BOOST_CHECK_EQUAL( test.str(), "12 ASD (10.2 10.2 10.2 10.2) 15.3 (0 0)" );
	}
	{
		stock_minute_data smd( quote_message::create_test_message( 190u, "DSA", 10.2, 15.3, 13.2, 14.7 ) );
		std::stringstream test;
		test << smd;
		BOOST_CHECK_EQUAL( test.str(), "190 DSA (0 0 0 0) 0 (15.3 14.7)" );
	}
	{
		stock_minute_data smd( trade_message::create_test_message( 12u, "ASD", 10.2, 15.3 ) );
		smd.add_market_data( trade_message::create_test_message( 11u, "ASD", 13.1, 12.0 ) );
		smd.add_market_data( trade_message::create_test_message( 14u, "ASD", 16.1, 18.0 ) );
		smd.add_market_data( trade_message::create_test_message( 13u, "ASD", 9.1, 4.7 ) );
		smd.add_market_data( quote_message::create_test_message( 190u, "ASD", 10.2, 15.3, 13.2, 13.7 ) );
		smd.add_market_data( quote_message::create_test_message( 190u, "ASD", 10.2, 14.7, 13.2, 12.4 ) );
		std::stringstream test;
		test << smd;
		BOOST_CHECK_EQUAL( test.str(), "11 ASD (13.1 16.1 9.1 16.1) 50 (30 26.1)" );
	}
	{
		std::ofstream out( BINARY_DIR "/test_output.bin", std::ios::binary );
		stock_minute_data smd( trade_message::create_test_message( 12u, "ASD", 10.2, 15.3 ) );
		smd.print_binary( out );
		out.close();

		BOOST_CHECK_EQUAL( boost::filesystem::file_size( BINARY_DIR "/test_output.bin" ), 76ul );
		boost::filesystem::remove( BINARY_DIR "/test_output.bin" );
	}
}
