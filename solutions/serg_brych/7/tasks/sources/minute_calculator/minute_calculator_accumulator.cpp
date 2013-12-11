#include "minute_calculator_accumulator.h"
#include <boost/algorithm/string.hpp>

minute_calculator::minute_calculator_accumulator::minute_calculator_accumulator(callback_type callback):current_minute_(0),callback_(callback)
{

}

minute_calculator::minute_calculator_accumulator::~minute_calculator_accumulator()
{
	stop();
}

void minute_calculator::minute_calculator_accumulator::run()
{
	process_threads_.create_thread(boost::bind(&minute_calculator::minute_calculator_accumulator::calculate_trade, this));
	process_threads_.create_thread(boost::bind(&minute_calculator::minute_calculator_accumulator::calculate_quote, this));
}

void minute_calculator::minute_calculator_accumulator::stop()
{
	trade_queue_.stop();
	quote_queue_.stop();
	
	process_threads_.join_all();
}

bool minute_calculator::minute_calculator_accumulator::push_trade(const multicast_communication::trade_message_ptr& msg)
{
	multicast_communication::trade_message *tmp = new multicast_communication::trade_message();
	*tmp = *msg.get();
	return trade_queue_.push(tmp);
}

bool minute_calculator::minute_calculator_accumulator::push_quote(const multicast_communication::quote_message_ptr& msg)
{
	multicast_communication::quote_message *tmp = new multicast_communication::quote_message();
	*tmp = *msg.get();
	return quote_queue_.push(tmp);
}

void minute_calculator::minute_calculator_accumulator::minute_msg()
{
	boost::shared_ptr<container_minute_data> transfer_data_ptr;
	{
		boost::mutex::scoped_lock lock(lock_data_);
		if(calculate_data_.empty())
			return;
		transfer_data_ptr.reset( new container_minute_data(calculate_data_.begin(), calculate_data_.end()));
		calculate_data_.clear();
	}
	callback_(transfer_data_ptr);
}

void minute_calculator::minute_calculator_accumulator::calculate_trade()
{
	while(true)
	{
		multicast_communication::trade_message_ptr msg(trade_queue_.wait_pop());
		if(!msg)
		{
			break;
		}
		{
			uint32_t new_time = msg->time()/60;
			if(new_time > current_minute_)
			{
				current_minute_ = new_time;
				minute_msg();
			}
			std::string key = msg->security_symbol();
			
			double msg_price = msg->price();
			
			boost::trim(key);
			{
				boost::mutex::scoped_lock lock(lock_data_);
				if( calculate_data_.find(key) == calculate_data_.end())
				{
					calculate_data_[key].open_price = msg_price;
					calculate_data_[key].low_price = msg_price;
					calculate_data_[key].stock_name = key;
					calculate_data_[key].minute_ = new_time;
				}
			}

			if( calculate_data_[key].high_price < msg_price)
				calculate_data_[key].high_price = msg_price;
			
			if( calculate_data_[key].low_price > msg_price)
				calculate_data_[key].low_price = msg_price;

			calculate_data_[key].volume += msg_price;
			calculate_data_[key].close_price = msg_price;
		}
	}
}

void minute_calculator::minute_calculator_accumulator::calculate_quote()
{
	while(true)
	{
		multicast_communication::quote_message_ptr msg(quote_queue_.wait_pop());
		if(!msg)
		{
			break;
		}
		{
			uint32_t new_time = msg->time()/60;
			if(new_time > current_minute_)
			{
				current_minute_ = new_time;
				minute_msg();
			}

			std::string key = msg->security_symbol();
			double bid = msg->bid_volume();
			double ask = msg->offer_volume();
		
			boost::trim(key);
			{
				boost::mutex::scoped_lock lock(lock_data_);
				if( calculate_data_.find(key) == calculate_data_.end())
				{
					calculate_data_[key].stock_name = key;
					calculate_data_[key].minute_ = new_time;
				}
			}

			calculate_data_[key].bid += bid;
			calculate_data_[key].ask += ask;
		}
	}
}
