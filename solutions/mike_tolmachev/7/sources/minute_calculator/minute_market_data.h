#pragma once

#include "market_data_processor.h"
#include "market_minute_processor.h"

#include "minute_calculator.h"
#include "market_data_receiver.h"


namespace minute_calculator
{

class minute_market_data : public multicast_communication::market_data_processor,
							public market_minute_processor
{
	minute_calculator calculator_;
	multicast_communication::market_data_receiver receiver_;

	mutable boost::mutex mtx_;

	std::map<std::string, std::ofstream> outputs_;

	std::string output_dir_;

public:
	explicit minute_market_data(const std::string& config_file, const std::string& output_dir);
	virtual ~minute_market_data();
	
private:
	virtual void new_trade( const multicast_communication::trade_message_ptr& trade);
	virtual void new_quote( const multicast_communication::quote_message_ptr& quote);

	virtual void new_minute(const minute_data& minute);
};

}