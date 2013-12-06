#include <address.h>

common::address::address(const std::string& ip, const unsigned short port)
: ip_( ip )
, port_( port )
{
}

boost::asio::ip::udp::endpoint common::address::endpoint() const
{
	return boost::asio::ip::udp::endpoint( boost::asio::ip::address::from_string( ip_ ), port_ );
}
