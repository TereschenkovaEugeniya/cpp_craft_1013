#include "minute_calculator.h"

namespace minute_calculator
{
	std::ostream& operator<<(std::ostream& output, const minute_data& md)
	{		
		output.write(reinterpret_cast< const char* >(&md.minute), sizeof( md.minute));
		output.write(md.stock_name, 16);
		output.write(reinterpret_cast< const char* >(&md.open_prise), sizeof( md.open_prise));
		output.write(reinterpret_cast< const char* >(&md.high_prise), sizeof( md.high_prise));
		output.write(reinterpret_cast< const char* >(&md.low_price), sizeof( md.low_price));
		output.write(reinterpret_cast< const char* >(&md.close_prise), sizeof( md.close_prise));
		output.write(reinterpret_cast< const char* >(&md.volume), sizeof( md.volume));
		output.write(reinterpret_cast< const char* >(&md.bid), sizeof( md.bid));
		output.write(reinterpret_cast< const char* >(&md.ask), sizeof( md.ask));

		return output;
	}

	minute_calculator::minute_calculator(market_minute_processor& processor) : processor_(processor)
	{
	}

	void minute_calculator::new_trade( const multicast_communication::trade_message_ptr& msg)
	{
		boost::mutex::scoped_lock lock(mtx_);

		minute_data minute;
		std::memset(&minute, 0, sizeof(minute));
		std::string security_symbol = msg->security_symbol();

		if (minutes_.find(security_symbol) != minutes_.end())
		{
			minute = minutes_[security_symbol];			
		}
		else
		{
			new_minute(minute, msg);
		}		

		if (msg->time() >= (minute.minute + 60))
		{
			//callback
			processor_.new_minute(minute);

			std::memset(&minute, 0, sizeof(minute));
			new_minute(minute, msg);
		}

		if (!minute.init_by_trade)
		{
			minute.open_prise = msg->price();
			minute.low_price = msg->price();
		}

		if (minute.high_prise < msg->price())
		{
			minute.high_prise = msg->price();
		}
		if (minute.low_price > msg->price())
		{
			minute.low_price = msg->price();
		}
		minute.volume += msg->volume();
		minute.close_prise = msg->price();

		minutes_[security_symbol] = minute;
	}

	void minute_calculator::new_quote( const multicast_communication::quote_message_ptr& msg)
	{
		boost::mutex::scoped_lock lock(mtx_);

		minute_data minute;
		std::memset(&minute, 0, sizeof(minute));
		std::string security_symbol = msg->security_symbol();

		if (minutes_.find(security_symbol) != minutes_.end())
		{
			minute = minutes_[security_symbol];
		}
		else
		{
			new_minute(minute, msg);
		}

		if (msg->time() >= (minute.minute + 60))
		{
			//callback
			processor_.new_minute(minute);

			std::memset(&minute, 0, sizeof(minute));
			new_minute(minute, msg);
		}

		minute.bid += msg->bid_volume();
		minute.ask += msg->offer_volume();

		minutes_[security_symbol] = minute;
	}

	void minute_calculator::new_minute(minute_data& minute, const multicast_communication::trade_message_ptr& msg)
	{
		std::string security_symbol = msg->security_symbol();

		minute.minute = msg->time();
		std::memcpy(minute.stock_name, security_symbol.c_str(), security_symbol.length());
		minute.open_prise = msg->price();
		minute.low_price = msg->price();

		minute.init_by_trade = true;
	}

	void minute_calculator::new_minute(minute_data& minute, const multicast_communication::quote_message_ptr& msg)
	{
		std::string security_symbol = msg->security_symbol();

		minute.minute = msg->time();
		std::memcpy(minute.stock_name, security_symbol.c_str(), security_symbol.length());
	}
}