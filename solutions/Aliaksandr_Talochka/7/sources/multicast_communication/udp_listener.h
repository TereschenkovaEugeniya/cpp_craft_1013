#ifndef _ASYNC_UDP_UDP_LISTENER_H_
#define _ASYNC_UDP_UDP_LISTENER_H_

#include <string>
#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "parser_quote.h"
#include "parser_trade.h"


namespace multicast_communication
{

	template <typename T>
		void push_vector(std::vector<T>& Target, const std::vector<T>& From);


	class udp_listener : virtual private boost::noncopyable
	{
		static const size_t default_buffer_size;

		boost::asio::io_service& io_service_;

		boost::asio::ip::udp::endpoint listen_endpoint_;
		boost::asio::ip::udp::socket socket_;

		std::string multicast_address_;

		typedef boost::shared_ptr< std::string > buffer_type;

		mutable boost::mutex protect_messages_;
		std::vector< messages::quote_message > messages_quote;
		std::vector< messages::trade_message > messages_trade;


	public:
		explicit udp_listener( boost::asio::io_service& io_service, const std::string& multicast_address, unsigned short port, const char data_type );
		~udp_listener();
		const std::vector< messages::quote_message > get_quote_messages();
		const std::vector< messages::trade_message > get_trade_messages();

	private:
		void socket_reload_();
		void register_listen_( buffer_type pre_buffer = buffer_type(), const size_t previous_size = 0 );
		void listen_handler_( buffer_type bt, const boost::system::error_code& error, const size_t bytes_received );
		static void enlarge_buffer_( buffer_type& bt );
		parser_messages::parser_quote quote_parser;
		parser_messages::parser_trade trade_parser;
		char data_type_;

	};
}

#endif // _ASYNC_UDP_UDP_LISTENER_H_
