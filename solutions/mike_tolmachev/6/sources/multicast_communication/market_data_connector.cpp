#include "market_data_connector.h"

namespace multicast_communication
{

market_data_connector::market_data_connector( boost::asio::io_service& io_service, const std::string& multicast_address, unsigned short port, task5_6::thread_safe_queue<std::string>& messages/*safe_queue_ptr& messages*/ )
	: io_service_( io_service )
	, listen_endpoint_( boost::asio::ip::address::from_string( "0.0.0.0" ), port )
	, socket_( io_service_ )
	, multicast_address_( multicast_address )
	, messages_(messages)
{
	socket_reload_();
	register_listen_();
}

market_data_connector::~market_data_connector()
{
}

void market_data_connector::socket_reload_()
{
	using boost::asio::ip::udp;
	using boost::asio::ip::address;
	using boost::asio::ip::multicast::join_group;

	socket_.open( listen_endpoint_.protocol() );
	socket_.set_option( udp::socket::reuse_address( true ) );
	socket_.bind( listen_endpoint_ );
	socket_.set_option( join_group( address::from_string( multicast_address_ ) ) );
}

void market_data_connector::register_listen_()
{
	boost::shared_ptr<std::string> buf = boost::shared_ptr<std::string>(new std::string(1000, '\0'));

	char* const buf_start = &(*buf->begin());

	using namespace boost::asio::placeholders;
	socket_.async_receive(boost::asio::buffer(buf_start, 1000),
        boost::bind(&market_data_connector::listen_handler_, this, buf, _1, _2));
}

void market_data_connector::listen_handler_(boost::shared_ptr<std::string> buf, const boost::system::error_code& error, const size_t bytes_received)
{
	if ( error )
	{
		return;
	}
	else
	{
		messages_.push( std::string(buf->c_str(), bytes_received ) );

		register_listen_();
	}
}

} // namespace multicast_communication