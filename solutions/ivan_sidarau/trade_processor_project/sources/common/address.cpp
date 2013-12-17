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

std::string common::address::ip() const
{
	return ip_;
}
unsigned short common::address::port() const
{
	return port_;
}
