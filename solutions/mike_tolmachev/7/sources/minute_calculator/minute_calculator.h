#pragma once

#include "trade_message.h"
#include "quote_message.h"
#include "market_minute_processor.h"
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <map>

namespace minute_calculator
{	
	std::ostream& operator<<(std::ostream& output, const minute_data& md);

	class minute_calculator : boost::noncopyable
	{
		std::map<std::string, minute_data> minutes_;
		market_minute_processor& processor_;

		mutable boost::mutex mtx_;

	public:
		explicit minute_calculator(market_minute_processor& processor);

		void new_trade(const multicast_communication::trade_message_ptr& msg);
		void new_quote(const multicast_communication::quote_message_ptr& msg);

	private:
		void new_minute(minute_data& minute, const multicast_communication::trade_message_ptr& msg);
		void new_minute(minute_data& minute, const multicast_communication::quote_message_ptr& msg);
	};
}