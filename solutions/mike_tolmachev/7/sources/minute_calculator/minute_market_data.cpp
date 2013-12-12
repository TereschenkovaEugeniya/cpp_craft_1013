#include "minute_market_data.h"


namespace minute_calculator
{

minute_market_data::minute_market_data(const std::string& config_file, const std::string& output_dir) : calculator_(*this),
	receiver_(config_file, *this), output_dir_(output_dir)
{
}

minute_market_data::~minute_market_data()
{
}

void minute_market_data::new_trade( const multicast_communication::trade_message_ptr& trade)
{
	calculator_.new_trade(trade);
}

void minute_market_data::new_quote( const multicast_communication::quote_message_ptr& quote)
{
	calculator_.new_quote(quote);
}

void minute_market_data::new_minute(const minute_data& minute)
{
	boost::mutex::scoped_lock lock(mtx_);

	std::ofstream& output = outputs_[minute.stock_name];

	if(!output.is_open())
	{
		std::string path = output_dir_ + "/";
		path.append(minute.stock_name);
		path.append(".dat");
		output.open(path);
	}

	output << minute;
}

}