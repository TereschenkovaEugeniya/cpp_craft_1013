#include "test_registrator.h"

#include <boost/filesystem.hpp>
#include <boost/shared_array.hpp>

#include <processors_test_helper.h>

#include <market_data_receiver.h>
#include <cts_parser.h>

namespace multicast_communication
{
	namespace tests_
	{
		namespace detail
		{
			size_t send_cts_file( const std::string& file_name, const std::string& ip, const unsigned short port );
			size_t send_cqs_file( const std::string& file_name, const std::string& ip, const unsigned short port );
			size_t send_file_with_delimeter( const std::string& file_name, const std::string& ip, const unsigned short port, const char delimeter );
		}
	}
}

size_t multicast_communication::tests_::detail::send_cts_file( const std::string& file_name, const std::string& ip, const unsigned short port )
{
	return send_file_with_delimeter( file_name, ip, port, cts_parser::ETX );
}
size_t multicast_communication::tests_::detail::send_cqs_file( const std::string& file_name, const std::string& ip, const unsigned short port )
{
	return send_file_with_delimeter( file_name, ip, port, cqs_parser::ETX );
}
size_t multicast_communication::tests_::detail::send_file_with_delimeter( const std::string& file_name, const std::string& ip, const unsigned short port, const char delimeter )
{
	const std::string file_path = (SOURCE_DIR "/tests/data/udp_data/" + file_name);

	const size_t file_size = boost::filesystem::file_size( file_path );
	boost::shared_array< char > buffer( new char[ file_size ] );

	std::ifstream ifs( file_path.c_str(), std::ios::binary );
	ifs.read( buffer.get(), file_size );
	ifs.close();

	using namespace boost::asio::ip;

	boost::asio::io_service service;
	udp::endpoint endpoint( address::from_string( ip ), port ); 
	udp::socket socket( service, endpoint.protocol() );

	size_t sended_messages = 0;
	char* i_begin = buffer.get();
	char* end_of_buffer = buffer.get() + file_size;
	char* i_end = std::find( i_begin, end_of_buffer, delimeter );

	while ( i_begin != end_of_buffer )
	{
		++sended_messages;
		socket.send_to( boost::asio::buffer( i_begin, i_end - i_begin ), endpoint );

		i_begin = i_end + 1;
		i_end = std::find( i_begin, end_of_buffer, delimeter );
	}
	return sended_messages;
}

void multicast_communication::tests_::market_data_receiver_tests()
{
	static const size_t free_buffers = 4ul;

	BOOST_CHECK_NO_THROW
	(
		market_data_receiver_manager_test_helper processor;
		market_data_receiver mdr( processor, 1ul, 1ul, common::addresses(), common::addresses(), free_buffers );
	);

	BOOST_CHECK_NO_THROW
	(
	{
		const std::string trade_ip = "233.200.79.128";
		const unsigned short trade_port = 61128;
		common::addresses trade_addresses;
		trade_addresses.push_back( common::address( trade_ip, trade_port ) );

		market_data_receiver_manager_test_helper processor;
		market_data_receiver mdr( processor, 1ul, 0ul, trade_addresses, common::addresses(), free_buffers );

		const size_t sended_messages_size = detail::send_cts_file( trade_ip + ".udp", trade_ip, trade_port );
		const size_t received_messages = processor.cts_message_sizes_.size();

		BOOST_CHECK_EQUAL( received_messages <= sended_messages_size, true );
		BOOST_CHECK_EQUAL( received_messages > 0ul, true );
	}
	);
	BOOST_CHECK_NO_THROW
	(
	{
		const std::string quote_ip = "233.200.79.0";
		const unsigned short quote_port = 61000;
		common::addresses quote_addresses;
		quote_addresses.push_back( common::address( quote_ip, quote_port ) );

		market_data_receiver_manager_test_helper processor;
		market_data_receiver mdr( processor, 0ul, 1ul, common::addresses(), quote_addresses, free_buffers );

		const size_t sended_messages_size = detail::send_cqs_file( quote_ip + ".udp", quote_ip, quote_port );
		const size_t received_messages = processor.cqs_message_sizes_.size();

		BOOST_CHECK_EQUAL( received_messages <= sended_messages_size, true );
		BOOST_CHECK_EQUAL( received_messages > 0ul, true );
	}
	);
}
