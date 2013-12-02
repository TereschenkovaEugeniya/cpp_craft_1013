#include "udp_listener.h"

#include <boost/bind.hpp>

async_udp::udp_listener::udp_listener( boost::asio::io_service& io_service, const std::string& multicast_address, unsigned short port, callback_type callback )
	: io_service_( io_service )
	, listen_endpoint_( boost::asio::ip::address::from_string( "0.0.0.0" ), port )
	, socket_( io_service_ )
	, multicast_address_( multicast_address )
	, callback_(callback)
{
	buffer_.reset( new std::string( max_buffer_size, '\0' ) );
	socket_reload_();
	register_listen_();
}
async_udp::udp_listener::~udp_listener()
{
	socket_.close();
}

void async_udp::udp_listener::socket_reload_()
{
	using boost::asio::ip::udp;
	using boost::asio::ip::address;
	using boost::asio::ip::multicast::join_group;

	socket_.open( listen_endpoint_.protocol() );
	socket_.set_option( udp::socket::reuse_address( true ) );
	socket_.bind( listen_endpoint_ );
	socket_.set_option( join_group( address::from_string( multicast_address_ ) ) );
}

void async_udp::udp_listener::register_listen_()
{
	char* const buffer_start = &(*buffer_->begin());

	using namespace boost::asio::placeholders;
	socket_.async_receive(boost::asio::buffer(buffer_start, max_buffer_size), boost::bind( &udp_listener::listen_handler_, this, error, bytes_transferred ) );
}

void async_udp::udp_listener::listen_handler_(const boost::system::error_code& error, const size_t bytes_received )
{
	if ( error )
	{
		static const int NO_ENOUGHT_BUFFER = 234;
		if ( error.value() == NO_ENOUGHT_BUFFER )
		{
			register_listen_();
		}
		return;
	}
	else
	{
		{
			boost::mutex::scoped_lock lock( protect_messages_ );
			callback_( std::string(buffer_->c_str(), bytes_received) );
		}
		register_listen_();
	}
}
