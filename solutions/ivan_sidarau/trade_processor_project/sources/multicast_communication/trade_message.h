#ifndef _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_
#define _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_

#include <ostream>

#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/ptime.hpp>

#include <cts_protocol.h>

namespace multicast_communication
{
	class trade_message;

	typedef boost::shared_ptr< trade_message > trade_message_ptr;

	class trade_message
	{
		boost::posix_time::ptime message_time_;
		boost::uint64_t minute_;

		std::string security_symbol_;
		double price_;
		double volume_;
	public:
		explicit trade_message();
		explicit trade_message( const cts_protocol::message_header&, const cts_protocol::short_trade& );
		explicit trade_message( const cts_protocol::message_header&, const cts_protocol::long_trade& );

		const boost::posix_time::ptime& message_time() const;
		boost::uint64_t minute() const;
		const std::string& security_symbol() const;
		double price() const;
		double volume() const;

		static trade_message_ptr create_test_message( 
			const boost::uint64_t minute, 
			const std::string security_symbol, 
			double price, 
			double volume );
	};


	std::ostream& operator<<( std::ostream&, const trade_message& );
}

#endif // _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_
