#include "mediator.h"

minute_calculator::mediator::mediator( const std::string& filepath )
 : config_( filepath + "/configs.ini" )
 , stock_minute_storage_( filepath )
 , cts_parser_( stock_minute_storage_ )
 , cqs_parser_( stock_minute_storage_ )
{
	receiver_.reset( new multicast_communication::market_data_receiver( *this, 
			config_.trade_receive_threads_size(),
			config_.quote_receive_threads_size(),
			config_.trade_listen_addresses(),
			config_.quote_listen_addresses() ) );
}

minute_calculator::mediator::~mediator()
{
}
//
void minute_calculator::mediator::cts_line_message( const common::buffer_ptr ptr, const size_t size )
{
	cts_parser_.process_message( ptr, size );
}
void minute_calculator::mediator::cqs_line_message( const common::buffer_ptr ptr, const size_t size )
{
	cqs_parser_.process_message( ptr, size );
}
