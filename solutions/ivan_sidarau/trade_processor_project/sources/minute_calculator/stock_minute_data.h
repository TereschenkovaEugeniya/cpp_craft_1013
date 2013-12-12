#ifndef _MINUTE_CALCULATOR_STOCK_MINUTE_H_
#define _MINUTE_CALCULATOR_STOCK_MINUTE_H_

#include <ostream>

#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>

#include <trade_message.h>
#include <quote_message.h>

namespace minute_calculator
{
	class stock_minute_data;

	std::ostream& operator<<( std::ostream&, const stock_minute_data& );

	class stock_minute_data : virtual public boost::noncopyable
	{
		friend std::ostream& operator<<( std::ostream&, const stock_minute_data& );

		boost::uint32_t open_minute_;
		boost::uint32_t close_minute_;

		std::string name_;
		double open_price_;
		double high_price_;
		double low_price_;
		double close_price_;

		double volume_;

		double bid_;
		double ask_;
	public:
		explicit stock_minute_data( const multicast_communication::trade_message_ptr& );
		explicit stock_minute_data( const multicast_communication::quote_message_ptr& );
		//
		void add_trade( const multicast_communication::trade_message_ptr& trade );
		void add_quote( const multicast_communication::quote_message_ptr& quote );
	};

}

#endif // _MINUTE_CALCULATOR_STOCK_MINUTE_H_

