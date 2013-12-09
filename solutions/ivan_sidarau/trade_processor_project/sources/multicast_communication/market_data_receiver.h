#ifndef _MULTICAST_COMMUNICATION_MARKET_DATA_RECEIVER_H_
#define _MULTICAST_COMMUNICATION_MARKET_DATA_RECEIVER_H_

#include <vector>
#include <queue>

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <address.h>
#include <buffer.h>

#include <cts_parser.h>
#include <cqs_parser.h>

namespace multicast_communication
{
	class market_data_receiver_manager
	{
		friend class market_data_receiver;
	protected:
		explicit market_data_receiver_manager(){}
		virtual ~market_data_receiver_manager(){}
	private:
		virtual void cts_line_message( const common::buffer_ptr ptr, const size_t size ) = 0;
		virtual void cqs_line_message( const common::buffer_ptr ptr, const size_t size ) = 0;
	};
	class market_data_receiver : virtual protected boost::noncopyable
	{
		static const size_t free_buffers_size = 102400ul;

		market_data_receiver_manager& manager_;

		boost::asio::io_service trade_receive_service_;
		boost::asio::io_service quote_receive_service_;

		typedef boost::shared_ptr< boost::asio::ip::udp::socket > socket_ptr;

		typedef std::vector< socket_ptr > sockets;
		sockets trade_sockets_;
		sockets quote_sockets_;

		boost::thread_group trade_receivers_;
		boost::thread_group quote_receivers_;

		boost::mutex buffers_protector_;
		std::queue< common::buffer_ptr > free_buffers_;

		typedef void (market_data_receiver::*on_new_message_ptr_)(socket_ptr socket, common::buffer_ptr received_buffer, const boost::system::error_code& error, const size_t bytes_received );

	public:
		explicit market_data_receiver(
			market_data_receiver_manager& manager,
			const size_t trades_receive_threads_size,
			const size_t quotes_receive_threads_size,
			const common::addresses& trade_addresses,
			const common::addresses& quote_addresses,
			const size_t default_buffers_size = free_buffers_size);
		~market_data_receiver();

		void stop();
		void insert_buffer( common::buffer_ptr new_buffer );
	private:
		void create_sockets_( const common::addresses& addresses, boost::asio::io_service& service, on_new_message_ptr_ functor, sockets& socket_array );
		//
		void connect_socket_( socket_ptr& socket, const common::address& to );
		void register_listen_( socket_ptr& socket, on_new_message_ptr_ functor );
		//
		void trade_thread_method_();
		void quote_thread_method_();
		//
		void on_new_cts_message_( socket_ptr socket, common::buffer_ptr received_buffer, const boost::system::error_code& error, const size_t bytes_received );
		void on_new_cqs_message_( socket_ptr socket, common::buffer_ptr received_buffer, const boost::system::error_code& error, const size_t bytes_received );
	};
}

#endif // _MULTICAST_COMMUNICATION_MARKET_DATA_RECEIVER_H_
