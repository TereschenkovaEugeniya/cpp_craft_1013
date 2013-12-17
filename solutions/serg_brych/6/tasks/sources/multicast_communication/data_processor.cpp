#include "data_processor.h"

multicast_communication::data_processor::data_processor(std::ostream& output):output_(output)
{

}

multicast_communication::data_processor::~data_processor()
{

}

void multicast_communication::data_processor::new_trade( const trade_message_ptr& msg)
{
	boost::lock_guard<boost::mutex> l(mutex_);
	output_ << *msg;
}

void multicast_communication::data_processor::new_quote( const quote_message_ptr& msg)
{
	boost::lock_guard<boost::mutex> l(mutex_);
	output_ << *msg;
}