#include <market_datafeed_writer.h>

#include <fstream>

multicast_communication::market_datafeed_writer::market_datafeed_writer( const std::string& filename )
: log_writer_( writing_threads_size, true )
, output_( filename, log_writer_, std::ios::out )
{
}

multicast_communication::market_datafeed_writer::~market_datafeed_writer()
{
	log_writer_.wait();
}

void multicast_communication::market_datafeed_writer::new_trade( const trade_message_ptr& trade )
{
	output_.note() << *trade;
}

void multicast_communication::market_datafeed_writer::new_quote( const quote_message_ptr& quote )
{
	output_.note() << *quote;
}

