#pragma once

#include "market_data_processor.h"
#include "market_minute_processor.h"

#include "minute_calculator.h"
#include "market_data_receiver.h"


class minute_market_data : public multicast_communication::market_data_processor,
							public minute_calculator::market_minute_processor
{
public:
	explicit minute_market_data(const std::string& config_file);
	virtual ~minute_market_data();

private:
	virtual void new_trade( const multicast_communication::trade_message_ptr& trade);
	virtual void new_quote( const multicast_communication::quote_message_ptr& quote);

	virtual void new_minute(const minute_calculator::minute_data& minute);
};