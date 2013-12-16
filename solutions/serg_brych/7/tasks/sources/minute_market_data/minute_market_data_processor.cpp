#include "minute_market_data_processor.h"

minute_market_data::minute_market_data_processor::minute_market_data_processor():stop_(false)
{
	receive_.reset(new multicast_communication::market_data_receiver(*this, new multicast_communication::config_reader(config_file_path)));
	calculator_.reset(new minute_calculator::minute_calculator_accumulator([&](minute_calculator::container_minute_data_ptr &data_ptr)
	{
		boost::unique_lock<boost::mutex> l(data_mutex_);
		output_.push_back(data_ptr);
		cv_.notify_one();
	}
	));

	write_thread_ptr_.reset(nullptr);
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
		if(stop_)
		{
			return;
		}
		
		if(output_.empty())
		{
			cv_.wait(l);
		}
				
		while( !output_.empty())
		{
			minute_calculator::container_minute_data_ptr to_out = output_.back();
			output_.pop_back();
		
			l.unlock();
			for( minute_calculator::container_minute_data::iterator it = to_out->begin(); it != to_out->end(); it++)
			{
				std::ofstream& output = out_[it->first];
				if(!output.is_open())
				{
					std::stringstream path_out; 
					path_out << BINARY_DIR << "\\" << it->first << ".dat";
					output.open( path_out.str() );
				}
				output << it->second;
			}
			l.lock();
		}
	}
}

void minute_market_data::minute_market_data_processor::run()
{
	receive_->run();
	calculator_->run();
	write_thread_ptr_.reset( new boost::thread(boost::bind(&minute_market_data::minute_market_data_processor::disk_writer, this)));
}

void minute_market_data::minute_market_data_processor::stop()
{
	receive_->stop();
	calculator_->stop();
	stop_ = true;
	cv_.notify_one();
	if(write_thread_ptr_.get())
		write_thread_ptr_->join();
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