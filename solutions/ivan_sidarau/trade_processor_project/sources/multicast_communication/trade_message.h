#ifndef _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_
#define _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_

#include <boost/shared_ptr.hpp>

#include <cts_protocol.h>

namespace multicast_communication
{
	class trade_message
	{
		std::string security_symbol_;
		double price_;
		double volume_;

		size_t time_;
	public:
		explicit trade_message();
		explicit trade_message( const cts_protocol::message_header&, const cts_protocol::short_trade& );
		explicit trade_message( const cts_protocol::message_header&, const cts_protocol::long_trade& );

		const std::string& security_symbol() const;
		double price() const;
		double volume() const;
	};

	typedef boost::shared_ptr< trade_message > trade_message_ptr;
}

#endif // _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_
