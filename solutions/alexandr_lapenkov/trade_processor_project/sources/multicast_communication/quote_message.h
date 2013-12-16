#ifndef _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_
#define _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_

#include "binreader.h"

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace multicast_communication
{
	class quote_message;

	typedef boost::shared_ptr< quote_message > quote_message_ptr;

	class quote_message : public boost::enable_shared_from_this< quote_message >
	{
		friend class processor;
		boost::function< void( const quote_message_ptr& ) > handler;

	private:
		std::string security_symbol,
			bid_price,
			bid_price_denominator_indicator,
			bid_size_of_units_of_trade,
			offer_price,
			offer_price_denominator_indicator,
			offer_size_of_units_of_trade,
			lower_limit_price_band_denominator_indicator, 
			lower_limit_price_band,
			bid_size_in_units_of_trade, 
			upper_limit_price_band_denominator_indicator,
			upper_limit_price_band,
			offer_size_in_units_of_trade;

	public:
		explicit quote_message( boost::function< void( const quote_message_ptr& ) > handler );

		std::string get_security_symbol() const;
		double get_bid_price() const;
		double get_bid_volume() const;
		double get_offer_price() const;
		double get_offer_volume() const;
		void parse( binreader& in, const char category, const char type );

	};

}

#endif // _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_
