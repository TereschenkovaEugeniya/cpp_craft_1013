#ifndef _MINUTE_CALCULATOR_STOCK_MINUTE_DATA_H_
#define _MINUTE_CALCULATOR_STOCK_MINUTE_DATA_H_

#include <ostream>

#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/ptime.hpp>

#include <trade_message.h>
#include <quote_message.h>

namespace minute_calculator
{
	class stock_minute_data;

	std::ostream& operator<<( std::ostream&, const stock_minute_data& );

	class stock_minute_data : virtual public boost::noncopyable
	{
		friend std::ostream& operator<<( std::ostream&, const stock_minute_data& );

		boost::posix_time::ptime minute_time_;

		boost::uint64_t open_minute_;
		boost::uint64_t close_minute_;

		static const size_t name_length = 16;
		static const char empty[ name_length ];

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
		const boost::posix_time::ptime& minute_time() const;
		//
		void add_market_data( const multicast_communication::trade_message_ptr& trade );
		void add_market_data( const multicast_communication::quote_message_ptr& quote );
		//
		void print_binary( std::ostream& out ) const;
	};

	typedef boost::shared_ptr< stock_minute_data > stock_minute_data_ptr;

}

#endif // _MINUTE_CALCULATOR_STOCK_MINUTE_DATA_H_

