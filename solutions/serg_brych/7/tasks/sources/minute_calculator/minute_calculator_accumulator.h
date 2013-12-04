#ifndef _MINUTE_CALCULATOR_ACCUMULATOR_H_
#define _MINUTE_CALCULATOR_ACCUMULATOR_H_

#include <map>
#include <string>
#include <cstdint>
#include <boost/thread.hpp>
#include <ts_queue.h>

#include <trade_message.h>
#include <quote_message.h>

namespace minute_calculator
{
	struct minute_data
	{
		minute_data():minute_(0),stock_name(""), open_price(0.0), high_price(0.0),low_price(0.0),close_price(0.0), volume(0.0), bid(0.0), ask(0.0)
		{
		
		}
		uint32_t minute_;

		std::string stock_name;
		double open_price;
		double high_price;
		double low_price;
		double close_price;
		double volume;

		double bid; // quote.bid_price()
		double ask; // quote.offer_price()
	};
	
	typedef system_utilities::common::ts_queue< multicast_communication::quote_message > quote_queue;
	typedef system_utilities::common::ts_queue< multicast_communication::trade_message > trade_queue;

	typedef std::map<std::string, minute_data> container_minute_data;

 	class minute_calculator_accumulator
	{
		boost::mutex lock_data_;
		quote_queue quote_queue_;
		trade_queue trade_queue_;

		boost::thread_group process_threads_;
		uint32_t current_minute_;
		
		container_minute_data calculate_data_;
	public:
		void run();
		void stop();
		minute_calculator_accumulator();
		~minute_calculator_accumulator();
		bool push_trade(const multicast_communication::trade_message_ptr&);
		bool push_quote(const multicast_communication::quote_message_ptr&);
		void minute_msg();
		void calculate_trade();
		void calculate_quote();
	};
}


#endif //_MINUTE_CALCULATOR_ACCUMULATOR_H_