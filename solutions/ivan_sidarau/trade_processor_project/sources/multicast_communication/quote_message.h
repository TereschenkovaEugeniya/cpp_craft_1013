#ifndef _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_
#define _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_

#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>

#include <cqs_protocol.h>

namespace multicast_communication
{
	class quote_message;

	typedef boost::shared_ptr< quote_message > quote_message_ptr;

	class quote_message
	{
		boost::uint32_t minute_;

		std::string security_symbol_;
		double bid_price_;
		double bid_volume_;
		double offer_price_;
		double offer_volume_;
	public:
		explicit quote_message();
		explicit quote_message( const cqs_protocol::message_header&, const cqs_protocol::short_quote& );
		explicit quote_message( const cqs_protocol::message_header&, const cqs_protocol::long_quote& );

		boost::uint32_t minute() const;
		const std::string& security_symbol() const;
		double bid_price() const;
		double bid_volume() const;
		double offer_price() const;
		double offer_volume() const;

		static quote_message_ptr create_test_message( 
			const boost::uint32_t minute, 
			const std::string security_symbol, 
			double bid_price, 
			double bid_volume, 
			double offer_price,
			double offer_volume );
	};

	std::ostream& operator<<( std::ostream&, const quote_message& );
}

#endif // _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_
