#include "minute_market_data_processor.h"

minute_market_data::minute_market_data_processor::minute_market_data_processor()
{

}

minute_market_data::minute_market_data_processor::~minute_market_data_processor()
{

}

void minute_market_data::minute_market_data_processor::new_trade( const multicast_communication::trade_message_ptr& msg)
{
	boost::lock_guard<boost::mutex> l(mutex_);

}

void minute_market_data::minute_market_data_processor::new_quote( const multicast_communication::quote_message_ptr& msg)
{
	boost::lock_guard<boost::mutex> l(mutex_);

}