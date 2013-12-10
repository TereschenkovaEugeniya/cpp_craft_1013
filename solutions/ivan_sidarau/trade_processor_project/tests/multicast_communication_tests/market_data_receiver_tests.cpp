#include "test_registrator.h"

#include <processors_test_helper.h>

#include <market_data_receiver.h>
#include <cts_parser.h>

namespace multicast_communication
{
	namespace tests_
	{
		namespace detail
		{
			class udp_send_functor
			{
				boost::asio::io_service service_;
				boost::asio::ip::udp::endpoint endpoint_;  
				mutable boost::asio::ip::udp::socket socket_;
			public:
				explicit udp_send_functor( const std::string& ip, unsigned short port )
					: service_()
					, endpoint_( boost::asio::ip::address::from_string( ip ), port )
					, socket_( service_, endpoint_.protocol() )
				{
				}
				void operator()( const char* const buffer, const size_t size ) const
				{
					socket_.send_to( boost::asio::buffer( buffer, size ), endpoint_ );
				}
			};
		}
	}
}


void multicast_communication::tests_::market_data_receiver_tests()
{
	static const size_t free_buffers = 4ul;

	BOOST_CHECK_NO_THROW
	(
		market_data_receiver_manager_test_helper processor;
		market_data_receiver mdr( processor, 1ul, 1ul, common::addresses(), common::addresses(), free_buffers );
	);

	{
		const std::string trade_ip = "233.200.79.128";
		const unsigned short trade_port = 61128;
		common::addresses trade_addresses;
		trade_addresses.push_back( common::address( trade_ip, trade_port ) );

		market_data_receiver_manager_test_helper processor;
		market_data_receiver mdr( processor, 4ul, 0ul, trade_addresses, common::addresses(), free_buffers );

		const size_t sended_messages_size = detail::send_cts_file( trade_ip + ".udp", detail::udp_send_functor( trade_ip, trade_port ) );
		const size_t received_messages = processor.cts_message_sizes_.size();

		BOOST_CHECK_EQUAL( sended_messages_size, 500ul );
		BOOST_CHECK_EQUAL( received_messages <= sended_messages_size, true );
		BOOST_CHECK_EQUAL( received_messages > 0ul, true );
	}
	{
		const std::string quote_ip = "233.200.79.0";
		const unsigned short quote_port = 61000;
		common::addresses quote_addresses;
		quote_addresses.push_back( common::address( quote_ip, quote_port ) );

		market_data_receiver_manager_test_helper processor;
		market_data_receiver mdr( processor, 0ul, 4ul, common::addresses(), quote_addresses, free_buffers );

		const size_t sended_messages_size = detail::send_cqs_file( quote_ip + ".udp", detail::udp_send_functor( quote_ip, quote_port ) );
		const size_t received_messages = processor.cqs_message_sizes_.size();

		BOOST_CHECK_EQUAL( sended_messages_size, 500ul );
		BOOST_CHECK_EQUAL( received_messages <= sended_messages_size, true );
		BOOST_CHECK_EQUAL( received_messages > 0ul, true );
	}
}
