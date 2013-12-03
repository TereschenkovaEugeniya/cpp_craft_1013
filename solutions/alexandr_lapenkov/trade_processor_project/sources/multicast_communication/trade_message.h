#ifndef _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_
#define _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_

#include "binreader.h"

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace multicast_communication
{
	class trade_message;

	typedef boost::shared_ptr< trade_message > trade_message_ptr;

	class trade_message : public boost::enable_shared_from_this< trade_message >
	{
	private:
		std::string security_symbol,
			trade_volume,
			price_denominator_indicator,
			trade_price;
		 boost::function< void( const trade_message_ptr& ) > handler;

	public:
		explicit trade_message( boost::function< void( const trade_message_ptr& ) > handler );

		std::string get_security_symbol() const;
		double get_price() const;
		double get_volume() const;
		void parse( binreader& in, const char category, const char type );

	};

}

#endif // _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_
