#include "minute_market_data_processor.h"

minute_market_data::minute_market_data_processor::minute_market_data_processor()
{
	receive_.reset(new multicast_communication::market_data_receiver(*this, new multicast_communication::config_reader(config_file_path)));
	calculator_.reset(new minute_calculator::minute_calculator_accumulator);
}

minute_market_data::minute_market_data_processor::~minute_market_data_processor()
{
	stop();
}

void minute_market_data::minute_market_data_processor::run()
{
	receive_->run();
	calculator_->run();
}

void minute_market_data::minute_market_data_processor::stop()
{
	receive_->stop();
	calculator_->stop();
}

void minute_market_data::minute_market_data_processor::new_trade( const multicast_communication::trade_message_ptr& msg)
{
	boost::lock_guard<boost::mutex> l(mutex_);
	//calculator_.push_trade(msg);
}

void minute_market_data::minute_market_data_processor::new_quote( const multicast_communication::quote_message_ptr& msg)
{
	boost::lock_guard<boost::mutex> l(mutex_);
	//calculator_.push_quote(msg);
}