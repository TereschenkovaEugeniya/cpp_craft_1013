#include "test_registrator.h"

#include <stock_minute_storage.h>

#include <fstream>

#include <boost/filesystem.hpp>

namespace minute_calculator
{
	namespace tests_
	{
		namespace detail
		{
			class stock_minute_storage_test_helper : public stock_minute_storage
			{
			public:
				explicit stock_minute_storage_test_helper();
				void new_message( const multicast_communication::trade_message_ptr& );
				void new_message( const multicast_communication::quote_message_ptr& );
			};
		}
	}
}

minute_calculator::tests_::detail::stock_minute_storage_test_helper::stock_minute_storage_test_helper()
 : stock_minute_storage( BINARY_DIR )
{
}

void minute_calculator::tests_::detail::stock_minute_storage_test_helper::new_message( const multicast_communication::trade_message_ptr& trade )
{
	new_data_( trade );
}
void minute_calculator::tests_::detail::stock_minute_storage_test_helper::new_message( const multicast_communication::quote_message_ptr& quote )
{
	new_data_( quote );
}


void minute_calculator::tests_::stock_minute_storage_tests()
{
	using multicast_communication::trade_message;
	using multicast_communication::quote_message;

	BOOST_CHECK_NO_THROW( stock_minute_storage( BINARY_DIR ); );

	{
		detail::stock_minute_storage_test_helper smsth;
		smsth.new_message( trade_message::create_test_message( common::create_uint_time( 14,55,25 ), "A", 14.15, 16.0 ) );
		smsth.new_message( trade_message::create_test_message( common::create_uint_time( 14,55,28 ), "A", 14.15, 16.0 ) );
		smsth.new_message( trade_message::create_test_message( common::create_uint_time( 14,56,02 ), "A", 14.15, 16.0 ) );
	}
	BOOST_CHECK_EQUAL( boost::filesystem::exists( BINARY_DIR "/A.dat" ), true );
	BOOST_CHECK_EQUAL( boost::filesystem::file_size( BINARY_DIR "/A.dat" ), 152ul );
	boost::filesystem::remove( BINARY_DIR "/A.dat" );

	{
		detail::stock_minute_storage_test_helper smsth;
		smsth.new_message( trade_message::create_test_message( common::create_uint_time( 14,55,25 ), "A", 14.15, 16.0 ) );
		smsth.new_message( trade_message::create_test_message( common::create_uint_time( 14,55,28 ), "B", 14.15, 16.0 ) );
		smsth.new_message( trade_message::create_test_message( common::create_uint_time( 14,56,02 ), "B", 14.15, 16.0 ) );
		smsth.new_message( trade_message::create_test_message( common::create_uint_time( 14,55,25 ), "B", 14.15, 16.0 ) );
		smsth.new_message( trade_message::create_test_message( common::create_uint_time( 14,55,28 ), "B", 14.15, 16.0 ) );
		smsth.new_message( trade_message::create_test_message( common::create_uint_time( 14,56,02 ), "B", 14.15, 16.0 ) );
	}
	BOOST_CHECK_EQUAL( boost::filesystem::exists( BINARY_DIR "/A.dat" ), true );
	BOOST_CHECK_EQUAL( boost::filesystem::file_size( BINARY_DIR "/A.dat" ), 76ul );
	BOOST_CHECK_EQUAL( boost::filesystem::exists( BINARY_DIR "/B.dat" ), true );
	BOOST_CHECK_EQUAL( boost::filesystem::file_size( BINARY_DIR "/B.dat" ), 152ul );
	boost::filesystem::remove( BINARY_DIR "/A.dat" );
	boost::filesystem::remove( BINARY_DIR "/B.dat" );

}
