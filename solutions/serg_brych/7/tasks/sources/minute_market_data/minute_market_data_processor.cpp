#include "minute_market_data_processor.h"

minute_market_data::minute_market_data_processor::minute_market_data_processor():stop_(false)
{
	receive_.reset(new multicast_communication::market_data_receiver(*this, new multicast_communication::config_reader(config_file_path)));
	calculator_.reset(new minute_calculator::minute_calculator_accumulator());
}

minute_market_data::minute_market_data_processor::~minute_market_data_processor()
{
	stop();
}

void  minute_market_data::minute_market_data_processor::disk_writer()
{
	while(true)
	{
		boost::unique_lock<boost::mutex> l(data_mutex_);

		if(output_.empty())
		{
			cv_.wait(l);
		}

		if(stop_)
		{
			while(!output_.empty())
			{
				output_.back();
				output_.pop_back();
			}
			return;
		}
		output_.back();
		output_.pop_back();
	}
}

void minute_market_data::minute_market_data_processor::minute_msg(minute_calculator::container_minute_data_ptr data_ptr)
{
	boost::unique_lock<boost::mutex> l(data_mutex_);
	output_.push_back(data_ptr);
	cv_.notify_one();
}

void minute_market_data::minute_market_data_processor::minute_market_data_processor::run()
{
	receive_->run();
	calculator_->run();
	write_thread_.start_thread(boost::bind(&minute_market_data::minute_market_data_processor::disk_writer, this));
}

void minute_market_data::minute_market_data_processor::stop()
{
	receive_->stop();
	calculator_->stop();
	stop_ = true;
	cv_.notify_one();
	write_thread_.join();
}

void minute_market_data::minute_market_data_processor::new_trade( const multicast_communication::trade_message_ptr& msg)
{
	boost::unique_lock<boost::mutex> l(mutex_);
	calculator_->push_trade(msg);
}

void minute_market_data::minute_market_data_processor::new_quote( const multicast_communication::quote_message_ptr& msg)
{
	boost::unique_lock<boost::mutex> l(mutex_);
	calculator_->push_quote(msg);
}