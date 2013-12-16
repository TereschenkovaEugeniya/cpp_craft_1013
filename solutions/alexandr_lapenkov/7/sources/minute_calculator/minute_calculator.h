#ifndef _MINUTE_CALCULATOR_MINUTE_CALCULATOR_H
#define _MINUTE_CALCULATOR_MINUTE_CALCULATOR_H

#include <trade_message.h>
#include <quote_message.h>

#include <vector>

#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/foreach.hpp>

namespace minute_calculator
{
	struct minute_datafeed
	{
		unsigned minute;
		char stock_name[16];
		double open_price
			, high_price
			, low_price
			, close_price
			, volume
			, bid
			, ask;

		minute_datafeed()
		{
			stock_name[0] = '\0';
		}

		minute_datafeed( std::string stock_name_ )
		{
			strcpy( stock_name, stock_name_.c_str() );
		}

	};

	typedef boost::shared_ptr< minute_datafeed > minute_datafeed_ptr;
	typedef boost::function< void( const minute_datafeed_ptr& datafeed ) > datafeed_callback_type;

	class datafeed_calculator
	{
		boost::unordered_map< std::string, minute_datafeed_ptr > data;
		datafeed_callback_type datafeed_handler;

	public:

		explicit datafeed_calculator( datafeed_callback_type handler );
		virtual ~datafeed_calculator();

		void new_trade( const multicast_communication::trade_message_ptr& );
		void new_quote( const multicast_communication::quote_message_ptr& );
	};

}

#endif //_MINUTE_CALCULATOR_MINUTE_CALCULATOR_H