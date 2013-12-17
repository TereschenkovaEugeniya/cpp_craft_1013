#include "test_registrator.h"

#include <cqs_parser.h>

#include <boost/regex.hpp>

#include <processors_test_helper.h>

namespace multicast_communication
{
	namespace tests_
	{
		namespace detail
		{
			class quote_processor_test_helper : public quote_processor
			{
			public:
				size_t amount_of_messages_;
				std::vector< std::string > stock_names_;
				std::vector< quote_message_ptr > messages_;

				explicit quote_processor_test_helper()
					: amount_of_messages_( 0ul )
				{}
				virtual ~quote_processor_test_helper(){}
			private:
				virtual void new_quote( quote_message_ptr& new_message );
			};
		}
	}
}

void multicast_communication::tests_::detail::quote_processor_test_helper::new_quote( quote_message_ptr& new_message )
{
	if ( new_message )
	{
		stock_names_.push_back( new_message->security_symbol() );
		messages_.push_back( new_message );
	}
	++amount_of_messages_;
}

void multicast_communication::tests_::cqs_parser_tests()
{
	{
		detail::quote_processor_test_helper qpth;
		BOOST_CHECK_NO_THROW( cqs_parser parser( qpth ); );
	}
	{
		detail::quote_processor_test_helper qpth;
		cqs_parser parser( qpth );

		boost::regex quote_file_check( "(233)\\.(200)\\.(79)\\.(\\d{1,2})\\.(udp)" );

		for ( boost::filesystem::directory_iterator iter( SOURCE_DIR "/tests/data/udp_data/" ), end ; iter != end ; ++iter )
		{
			const std::string filename = iter->path().filename().string();

			if ( boost::regex_match( filename, quote_file_check ) )
			{
				const size_t sended_messages_size = detail::send_cqs_file( filename, detail::send_to_processor< cqs_parser >( parser ) );

				BOOST_CHECK_EQUAL( qpth.amount_of_messages_ >= sended_messages_size, true );
				qpth.amount_of_messages_ = 0ul;
			}
		}
	}
}

void multicast_communication::tests_::cqs_message_parsing_tests()
{
	detail::quote_processor_test_helper qpth;
	cqs_parser parser( qpth );
	std::pair< common::buffer_ptr, size_t > m1 = detail::read_file( SOURCE_DIR "/tests/data/udp_messages/cqs_message_1.bin" );
	parser.process_message( m1.first, m1.second );
	BOOST_CHECK_EQUAL( qpth.amount_of_messages_, 0ul );

	std::pair< common::buffer_ptr, size_t > m2 = detail::read_file( SOURCE_DIR "/tests/data/udp_messages/cqs_message_2.bin" );
	parser.process_message( m2.first, m2.second );
	BOOST_CHECK_EQUAL( qpth.amount_of_messages_, 0ul );

	std::pair< common::buffer_ptr, size_t > m3 = detail::read_file( SOURCE_DIR "/tests/data/udp_messages/cqs_message_3.bin" );
	parser.process_message( m3.first, m3.second );
	BOOST_CHECK_EQUAL( qpth.amount_of_messages_, 1ul );
	BOOST_CHECK_EQUAL( qpth.messages_[ 0 ]->bid_price(), 11.28 );
	BOOST_CHECK_EQUAL( qpth.messages_[ 0 ]->bid_volume(), 57.0 );
	BOOST_CHECK_EQUAL( qpth.messages_[ 0 ]->offer_price(), 11.29 );
	BOOST_CHECK_EQUAL( qpth.messages_[ 0 ]->offer_volume(), 4.0 );
	BOOST_CHECK_EQUAL( qpth.messages_[ 0 ]->security_symbol(), "BBVA" );
	BOOST_CHECK_EQUAL( qpth.messages_[ 0 ]->minute(), 3596264807000076u );

	std::pair< common::buffer_ptr, size_t > m4 = detail::read_file( SOURCE_DIR "/tests/data/udp_messages/cqs_message_4.bin" );
	parser.process_message( m4.first, m4.second );
	BOOST_CHECK_EQUAL( qpth.amount_of_messages_, 3ul );

	BOOST_CHECK_EQUAL( qpth.messages_[ 1 ]->bid_price(), 104.84 );
	BOOST_CHECK_EQUAL( qpth.messages_[ 1 ]->bid_volume(), 3.0 );
	BOOST_CHECK_EQUAL( qpth.messages_[ 1 ]->offer_price(), 104.9 );
	BOOST_CHECK_EQUAL( qpth.messages_[ 1 ]->offer_volume(), 3.0 );
	BOOST_CHECK_EQUAL( qpth.messages_[ 1 ]->security_symbol(), "BUD" );
	BOOST_CHECK_EQUAL( qpth.messages_[ 1 ]->minute(), 3596264807000073u );

	BOOST_CHECK_EQUAL( qpth.messages_[ 2 ]->bid_price(), 60.24 );
	BOOST_CHECK_EQUAL( qpth.messages_[ 2 ]->bid_volume(), 3.0 );
	BOOST_CHECK_EQUAL( qpth.messages_[ 2 ]->offer_price(), 60.25 );
	BOOST_CHECK_EQUAL( qpth.messages_[ 2 ]->offer_volume(), 3.0 );
	BOOST_CHECK_EQUAL( qpth.messages_[ 2 ]->security_symbol(), "WAG" );
	BOOST_CHECK_EQUAL( qpth.messages_[ 2 ]->minute(), 3596264807000108u );
}

