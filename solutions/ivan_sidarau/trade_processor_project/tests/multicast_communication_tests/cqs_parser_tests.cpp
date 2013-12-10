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
		stock_names_.push_back( new_message->security_symbol() );
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
