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
			class send_to_processor
			{
				cts_parser& parser_;
			public:
				explicit send_to_processor( cts_parser& parser )
					: parser_( parser )
				{
				}
				void operator()( const char* const buff, const size_t size ) const
				{
					common::buffer_ptr ptr( new common::buffer() );
					memcpy( ptr->data, buff, size );

					parser_.process_message( ptr, size );
				}
			};

			class trade_processor_test_helper : public trade_processor
			{
			public:
				size_t amount_of_messages_;
				std::vector< std::string > stock_names_;

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
		stock_names_.push_back( new_message->security_symbol() );
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
				const size_t sended_messages_size = detail::send_cts_file( filename, detail::send_to_processor( parser ) );

				BOOST_CHECK_EQUAL( sended_messages_size, 500ul );
				BOOST_CHECK_EQUAL( tpth.amount_of_messages_ >= 499ul, true );
				BOOST_CHECK_EQUAL( tpth.amount_of_messages_ <= 500ul, true );
				tpth.amount_of_messages_ = 0ul;
			}
		}
	}
}

