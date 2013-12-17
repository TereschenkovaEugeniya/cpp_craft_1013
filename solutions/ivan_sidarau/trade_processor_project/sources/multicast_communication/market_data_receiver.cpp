#include "market_data_receiver.h"

multicast_communication::market_data_receiver::market_data_receiver(
	market_data_receiver_manager& manager,
	const size_t trades_receive_threads_size,
	const size_t quotes_receive_threads_size,
	const common::addresses& trade_addresses,
	const common::addresses& quote_addresses,
	const size_t default_buffers_size )
 : manager_( manager )
 , trade_receive_service_()
 , quote_receive_service_()
{
	for ( size_t i = 0 ; i < default_buffers_size ; ++i )	
		free_buffers_.push( common::buffer_ptr( new common::buffer() ) );

	create_sockets_( trade_addresses, trade_receive_service_, &market_data_receiver::on_new_cts_message_, trade_sockets_ );
	create_sockets_( quote_addresses, quote_receive_service_, &market_data_receiver::on_new_cqs_message_, quote_sockets_ );

	for( size_t i = 0 ; i < trades_receive_threads_size ; ++i )
		trade_receivers_.create_thread( boost::bind( &market_data_receiver::trade_thread_method_, this ) );
	for( size_t i = 0 ; i < quotes_receive_threads_size ; ++i )
		quote_receivers_.create_thread( boost::bind( &market_data_receiver::quote_thread_method_, this ) );
}

multicast_communication::market_data_receiver::~market_data_receiver()
{
	stop();
}
//
void multicast_communication::market_data_receiver::stop()
{
	trade_receive_service_.stop();
	quote_receive_service_.stop();

	trade_receivers_.join_all();
	quote_receivers_.join_all();
}
void multicast_communication::market_data_receiver::insert_buffer( common::buffer_ptr new_buffer )
{
	boost::mutex::scoped_lock lock( buffers_protector_ );
	free_buffers_.push( new_buffer );
}
//
void multicast_communication::market_data_receiver::create_sockets_( 
	const common::addresses& addresses, boost::asio::io_service& service, on_new_message_ptr_ functor, sockets& socket_array )
{
	for ( common::addresses::const_iterator i = addresses.begin() ; i != addresses.end() ; ++i )
	{
		socket_ptr listen_socket( new boost::asio::ip::udp::socket( service ) );
		connect_socket_( listen_socket, *i );
		register_listen_( listen_socket, functor );
		socket_array.push_back( listen_socket );
	}
}
//
void multicast_communication::market_data_receiver::connect_socket_( socket_ptr& socket, const common::address& to )
{
	using boost::asio::ip::udp;
	using boost::asio::ip::address;
	using boost::asio::ip::multicast::join_group;

	udp::endpoint listen_endpoint( address::from_string( "0.0.0.0" ), to.port() );

	socket->open( listen_endpoint.protocol() );
	socket->set_option( udp::socket::reuse_address( true ) );
	socket->bind( listen_endpoint );
	socket->set_option( join_group( address::from_string( to.ip() ) ) );
}
void multicast_communication::market_data_receiver::register_listen_( socket_ptr& socket, on_new_message_ptr_ functor )
{
	using namespace boost::asio::placeholders;

	common::buffer_ptr buffer_to;
	{
		boost::mutex::scoped_lock lock( buffers_protector_ );
		if ( free_buffers_.empty() )
			buffer_to = common::buffer_ptr( new common::buffer() );
		else
		{
			buffer_to = free_buffers_.back();
			free_buffers_.pop();
		}
	}

	socket->async_receive( boost::asio::buffer( buffer_to->data, common::buffer::size ), 
		boost::bind( functor, this, socket, buffer_to, error, bytes_transferred ) );
}
//
void multicast_communication::market_data_receiver::trade_thread_method_()
{
	trade_receive_service_.run();
}
void multicast_communication::market_data_receiver::quote_thread_method_()
{
	quote_receive_service_.run();
}

//
void multicast_communication::market_data_receiver::on_new_cts_message_( socket_ptr socket, common::buffer_ptr received_buffer, const boost::system::error_code& error, const size_t bytes_received )
{
	if ( !error )
	{
		manager_.cts_line_message( received_buffer, bytes_received );
		return_buffer_( received_buffer );
		register_listen_( socket, &market_data_receiver::on_new_cts_message_ );
	}
}
void multicast_communication::market_data_receiver::on_new_cqs_message_( socket_ptr socket, common::buffer_ptr received_buffer, const boost::system::error_code& error, const size_t bytes_received )
{
	if ( !error )
	{
		manager_.cqs_line_message( received_buffer, bytes_received );
		return_buffer_( received_buffer );
		register_listen_( socket, &market_data_receiver::on_new_cqs_message_ );
	}
}
//
void multicast_communication::market_data_receiver::return_buffer_( common::buffer_ptr processed_buffer )
{
	boost::mutex::scoped_lock lock( buffers_protector_ );
	free_buffers_.push( processed_buffer );
}
