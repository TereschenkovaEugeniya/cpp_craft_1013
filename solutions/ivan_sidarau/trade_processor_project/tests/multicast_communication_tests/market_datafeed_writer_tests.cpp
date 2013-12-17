#include "test_registrator.h"

#include <market_datafeed_writer.h>

namespace multicast_communication
{
	namespace tests_
	{
		namespace detail
		{
			class market_datafeed_writer_test_helper : virtual protected boost::noncopyable
			{
				market_datafeed_writer writer_;
			public:
				explicit market_datafeed_writer_test_helper( const std::string& filename );
				void write( trade_message_ptr& trade );
				void write( quote_message_ptr& quote );
			};
		}
	}
}

multicast_communication::tests_::detail::market_datafeed_writer_test_helper::market_datafeed_writer_test_helper( const std::string& filename )
: writer_( filename )
{
}
void multicast_communication::tests_::detail::market_datafeed_writer_test_helper::write( trade_message_ptr& trade )
{
	writer_.new_trade( trade );
}
void multicast_communication::tests_::detail::market_datafeed_writer_test_helper::write( quote_message_ptr& quote )
{
	writer_.new_quote( quote );
}

void multicast_communication::tests_::market_datafeed_writer_tests()
{
	BOOST_CHECK_NO_THROW
	({
		market_datafeed_writer writer( BINARY_DIR "/test_file_name.txt" );
	});
	BOOST_CHECK_EQUAL( boost::filesystem::exists( BINARY_DIR "/test_file_name.txt" ), true );
	boost::filesystem::remove( BINARY_DIR "/test_file_name.txt" );
	{
		detail::market_datafeed_writer_test_helper writer( BINARY_DIR "/test_file_name.txt" );
		writer.write( trade_message_ptr( new trade_message() ) );
		writer.write( quote_message_ptr( new quote_message() ) );
	}
	BOOST_CHECK_EQUAL( boost::filesystem::exists( BINARY_DIR "/test_file_name.txt" ), true );
	BOOST_CHECK_EQUAL( boost::filesystem::file_size( BINARY_DIR "/test_file_name.txt" ), 20ul );
	boost::filesystem::remove( BINARY_DIR "/test_file_name.txt" );
}
