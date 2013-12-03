#ifndef _MINUTE_MARKET_DATA_PROCESSOR_H_
#define _MINUTE_MARKET_DATA_PROCESSOR_H_

#include <iostream>
#include <boost/thread.hpp>

#include "minute_calculator_accumulator.h"
#include "market_data_processor.h"

namespace minute_market_data
{

	class minute_market_data_processor : public multicast_communication::market_data_processor
	{
		boost::mutex mutex_;
	private:
		virtual void new_trade( const multicast_communication::trade_message_ptr& );
		virtual void new_quote( const multicast_communication::quote_message_ptr& );
	public:

		explicit minute_market_data_processor();
		~minute_market_data_processor();


	};
}

#endif //_MINUTE_MARKET_DATA_PROCESSOR_H_