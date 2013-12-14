#include "udp_listener.h"

#include <boost/bind.hpp>

const size_t multicast_communication::udp_listener::default_buffer_size = 1000ul;

multicast_communication::udp_listener::udp_listener( boost::asio::io_service& io_service, const std::string& multicast_address, unsigned short port, const char data_type)
	: io_service_( io_service )
	, listen_endpoint_( boost::asio::ip::address::from_string( "0.0.0.0" ), port )
	, socket_( io_service_ )
	, multicast_address_( multicast_address )
	, data_type_(data_type)
{
	socket_reload_();
	register_listen_();
}
multicast_communication::udp_listener::~udp_listener()
{
	if (socket_.is_open())
	{
		socket_.close();
	}
}

const std::vector< messages::quote_message > multicast_communication::udp_listener::get_quote_messages()
{
	boost::mutex::scoped_lock lock( protect_messages_ );

	std::vector< messages::quote_message > out = messages_quote;
	messages_quote.clear();
	return out;
}

const std::vector< messages::trade_message > multicast_communication::udp_listener::get_trade_messages()
{
	boost::mutex::scoped_lock lock( protect_messages_ );
	std::vector< messages::trade_message > out = messages_trade;
	return out;
}

void multicast_communication::udp_listener::socket_reload_()
{
	using boost::asio::ip::udp;
	using boost::asio::ip::address;
	using boost::asio::ip::multicast::join_group;

	socket_.open( listen_endpoint_.protocol() );
	socket_.set_option( udp::socket::reuse_address( true ) );
	socket_.bind( listen_endpoint_ );
	socket_.set_option( join_group( address::from_string( multicast_address_ ) ) );
}
void multicast_communication::udp_listener::register_listen_( buffer_type pre_buffer, const size_t previous_size )
{
	buffer_type buffer;
	
	if( pre_buffer )
		buffer = pre_buffer;
	else
		buffer.reset( new std::string( default_buffer_size, '\0' ) );

	char* const buffer_start = &(*buffer->begin());

	using namespace boost::asio::placeholders;
	socket_.async_receive( boost::asio::buffer( buffer_start, previous_size + default_buffer_size ), 
		boost::bind( &udp_listener::listen_handler_, this, buffer, error, bytes_transferred ) );
}

void multicast_communication::udp_listener::listen_handler_( buffer_type bt, const boost::system::error_code& error, const size_t bytes_received )
{
	if ( error )
	{
		static const int NO_ENOUGHT_BUFFER = 234;
		if ( error.value() == NO_ENOUGHT_BUFFER )
		{
			enlarge_buffer_( bt );
			register_listen_( bt, bytes_received );
		}
		return;
	}
	if ( bytes_received == bt->size() && (*bt)[ bytes_received - 1 ] != '\0' )
	{
		enlarge_buffer_( bt );
		register_listen_( bt, bytes_received );
	}
	else
	{
		{
			boost::mutex::scoped_lock lock( protect_messages_ );
			switch(data_type_)
			{
			case 'Q' :
				push_vector<messages::quote_message>(messages_quote, quote_parser.parse(std::string( bt->c_str(), bytes_received )));
				break;
			case 'T' :
				push_vector<messages::trade_message>(messages_trade, trade_parser.parse(std::string( bt->c_str(), bytes_received )));
				break;
			}
			
		}
		register_listen_();
	}
}

void multicast_communication::udp_listener::enlarge_buffer_( buffer_type& bt )
{
	bt->resize( bt->size() + default_buffer_size );
}

template <typename T>
void multicast_communication::push_vector(std::vector<T>& Target, const std::vector<T>& From)
{
	for(std::vector<T>::const_iterator it = From.begin(); it != From.end(); ++it)
	{
		Target.push_back(*it);
	}
}