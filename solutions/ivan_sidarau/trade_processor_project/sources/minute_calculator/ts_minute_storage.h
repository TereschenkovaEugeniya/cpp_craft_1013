#ifndef _MINUTE_CALCULATOR_TS_MINUTE_STORAGE_H_
#define _MINUTE_CALCULATOR_TS_MINUTE_STORAGE_H_

#include <boost/thread.hpp>
#include <boost/cstdint.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <trade_message.h>
#include <quote_message.h>

#include <stock_minute_data.h>

namespace minute_calculator
{
	class ts_minute_storage : virtual protected boost::noncopyable
	{
		boost::mutex protector_;
		stock_minute_data_ptr data_;
	public:
		explicit ts_minute_storage();
		~ts_minute_storage();

		stock_minute_data_ptr new_data( const multicast_communication::trade_message_ptr& trade );
		stock_minute_data_ptr new_data( const multicast_communication::quote_message_ptr& quote );

		stock_minute_data_ptr get_last_minute();
	private:
		template< typename data_type >
		stock_minute_data_ptr new_data_( data_type& );

	};

	typedef boost::shared_ptr< ts_minute_storage > ts_minute_storage_ptr;

	template< typename data_type >
	stock_minute_data_ptr ts_minute_storage::new_data_( data_type& market_message )
	{
		boost::mutex::scoped_lock lock( protector_ );
		if ( !data_ )
			data_.reset( new stock_minute_data( market_message ) );
		else
		{
			const boost::posix_time::time_duration& data_time = data_->minute_time().time_of_day();
			const boost::posix_time::time_duration& market_message_time = market_message->message_time().time_of_day();

			if ( data_time.minutes() == market_message_time.minutes() && 
				 data_time.hours() == market_message_time.hours() )
				data_->add_market_data( market_message );
			else
			{
				if ( data_time > market_message_time )
					return stock_minute_data_ptr();

				stock_minute_data_ptr result = data_;
				data_.reset( new stock_minute_data( market_message ) );
				return result;
			}
		}
		return stock_minute_data_ptr();
	}

}
		
#endif // _MINUTE_CALCULATOR_TS_MINUTE_STORAGE_H_
