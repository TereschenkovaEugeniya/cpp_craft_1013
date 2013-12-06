#ifndef _COMMON_ADDRESS_H_
#define _COMMON_ADDRESS_H_

#include <boost/asio.hpp>

namespace common
{
	class address
	{
		std::string ip_;
		unsigned short port_;
	public:
		explicit address( const std::string&, const unsigned short );
		boost::asio::ip::udp::endpoint endpoint() const;
	};

	typedef std::vector< address > addresses;
}

#endif // _COMMON_ADDRESS_H_
