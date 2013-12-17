#include "udp_listener.h"

#include <boost/bind.hpp>
namespace async_udp{

const size_t udp_listener::default_buffer_size = 1000;

udp_listener::udp_listener( boost::asio::io_service& io_service, const std::string& multicast_address, unsigned short port, sink_type sink )
    : sink_(sink)
	, io_service_( io_service )
	, listen_endpoint_( boost::asio::ip::address::from_string( "0.0.0.0" ), port )
	, socket_( io_service_ )
	, multicast_address_( multicast_address )
    //, buffer_(new std::string (default_buffer_size, 0))
{
	socket_reload_();
	register_listen_();
}
udp_listener::~udp_listener()
{
}

void udp_listener::socket_reload_()
{
	using boost::asio::ip::udp;
	using boost::asio::ip::address;
	using boost::asio::ip::multicast::join_group;

	socket_.open( listen_endpoint_.protocol() );
	socket_.set_option( udp::socket::reuse_address( true ) );
	socket_.bind( listen_endpoint_ );
	socket_.set_option( join_group( address::from_string( multicast_address_ ) ) );
}
void udp_listener::register_listen_(buffer_type lo_buffer)
{
    buffer_type buffer;
	
	if( lo_buffer )
		buffer = lo_buffer;
	else
		buffer.reset( new std::string( default_buffer_size, '\0' ) );
	
	char* const buffer_start = &(*buffer->begin());

	using namespace boost::asio::placeholders;
	socket_.async_receive( boost::asio::buffer( buffer_start, default_buffer_size ),  boost::bind( & udp_listener::listen_handler_, this, buffer, error, bytes_transferred ) );
}

void udp_listener::listen_handler_(buffer_type lo_buffer, const boost::system::error_code& error, const size_t bytes_received )
{
	if ( error )
	{
		return;
	}
    
	
	sink_( std::string(lo_buffer->c_str(), bytes_received) );
    register_listen_();
}

} //namespace async_udp