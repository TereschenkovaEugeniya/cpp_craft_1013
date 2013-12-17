#include "test_registrator.h"

#include <cts_parser.h>

#include <boost/regex.hpp>

#include <processors_test_helper.h>

namespace multicast_communication
{
	namespace tests_
	{
		namespace detail
		{
			class trade_processor_test_helper : public trade_processor
			{
			public:
				size_t amount_of_messages_;
				std::vector< std::string > stock_names_;
				std::vector< trade_message_ptr > messages_;

				explicit trade_processor_test_helper()
					: amount_of_messages_( 0ul )
				{}
				virtual ~trade_processor_test_helper(){}
			private:
				virtual void new_trade( trade_message_ptr& new_message );
			};
		}
	}
}

void multicast_communication::tests_::detail::trade_processor_test_helper::new_trade( trade_message_ptr& new_message )
{
	if ( new_message )
	{
		stock_names_.push_back( new_message->security_symbol() );
		messages_.push_back( new_message );
	}

	++amount_of_messages_;
}


void multicast_communication::tests_::cts_parser_tests()
{
	{
		detail::trade_processor_test_helper tpth;
		BOOST_CHECK_NO_THROW( cts_parser parser( tpth ); );
	}
	{
		detail::trade_processor_test_helper tpth;
		cts_parser parser( tpth );

		boost::regex trade_file_check( "(233)\\.(200)\\.(79)\\.(\\d{3})\\.(udp)" );

		for ( boost::filesystem::directory_iterator iter( SOURCE_DIR "/tests/data/udp_data/" ), end ; iter != end ; ++iter )
		{
			const std::string filename = iter->path().filename().string();

			if ( boost::regex_match( filename, trade_file_check ) )
			{
				const size_t sended_messages_size = detail::send_cts_file( filename, detail::send_to_processor< cts_parser >( parser ) );

				BOOST_CHECK_EQUAL( sended_messages_size, 500ul );
				BOOST_CHECK_EQUAL( tpth.amount_of_messages_ >= 499ul, true );
				BOOST_CHECK_EQUAL( tpth.amount_of_messages_ <= 500ul, true );
				tpth.amount_of_messages_ = 0ul;
			}
		}
	}
}

void multicast_communication::tests_::cts_message_parsing_tests()
{
	detail::trade_processor_test_helper tpth;
	cts_parser parser( tpth );
	std::pair< common::buffer_ptr, size_t > m1 = detail::read_file( SOURCE_DIR "/tests/data/udp_messages/cts_message_1.bin" );
	parser.process_message( m1.first, m1.second );
	BOOST_CHECK_EQUAL( tpth.amount_of_messages_, 0ul );

	std::pair< common::buffer_ptr, size_t > m2 = detail::read_file( SOURCE_DIR "/tests/data/udp_messages/cts_message_2.bin" );
	parser.process_message( m2.first, m2.second );
	BOOST_CHECK_EQUAL( tpth.amount_of_messages_, 0ul );

	std::pair< common::buffer_ptr, size_t > m3 = detail::read_file( SOURCE_DIR "/tests/data/udp_messages/cts_message_3.bin" );
	parser.process_message( m3.first, m3.second );
	BOOST_CHECK_EQUAL( tpth.amount_of_messages_, 1ul );
	BOOST_CHECK_EQUAL( tpth.messages_[ 0 ]->price(), 47.47);
	BOOST_CHECK_EQUAL( tpth.messages_[ 0 ]->volume(), 100.0 );
	BOOST_CHECK_EQUAL( tpth.messages_[ 0 ]->security_symbol(), "AEP" );
	BOOST_CHECK_EQUAL( tpth.messages_[ 0 ]->minute(), 3596263435000209u );

	std::pair< common::buffer_ptr, size_t > m4 = detail::read_file( SOURCE_DIR "/tests/data/udp_messages/cts_message_4.bin" );
	parser.process_message( m4.first, m4.second );
	BOOST_CHECK_EQUAL( tpth.amount_of_messages_, 4ul );

	BOOST_CHECK_EQUAL( tpth.messages_[ 1 ]->price(), 74.13 );
	BOOST_CHECK_EQUAL( tpth.messages_[ 1 ]->volume(), 100.0 );
	BOOST_CHECK_EQUAL( tpth.messages_[ 1 ]->security_symbol(), "NBL" );
	BOOST_CHECK_EQUAL( tpth.messages_[ 1 ]->minute(), 3596263425000127u );

	BOOST_CHECK_EQUAL( tpth.messages_[ 2 ]->price(), 47.47 );
	BOOST_CHECK_EQUAL( tpth.messages_[ 2 ]->volume(), 100.0 );
	BOOST_CHECK_EQUAL( tpth.messages_[ 2 ]->security_symbol(), "AEP" );
	BOOST_CHECK_EQUAL( tpth.messages_[ 2 ]->minute(), 3596263435000209u );

	BOOST_CHECK_EQUAL( tpth.messages_[ 3 ]->price(), 52.56 );
	BOOST_CHECK_EQUAL( tpth.messages_[ 3 ]->volume(), 100.0 );
	BOOST_CHECK_EQUAL( tpth.messages_[ 3 ]->security_symbol(), "ACMP" );
	BOOST_CHECK_EQUAL( tpth.messages_[ 3 ]->minute(), 3596263434000988u );
}

