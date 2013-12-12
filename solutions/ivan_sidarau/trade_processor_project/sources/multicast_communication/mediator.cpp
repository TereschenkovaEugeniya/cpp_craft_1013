#include <mediator.h>

//
multicast_communication::mediator::mediator( const std::string& filepath )
: config_( filepath + "/configs.ini" )
, writer_( filepath + "/market_data.dat" )
, cts_parser_( writer_ )
, cqs_parser_( writer_ )
{
	receiver_.reset( new market_data_receiver( *this, 
			config_.trade_receive_threads_size(),
			config_.quote_receive_threads_size(),
			config_.trade_listen_addresses(),
			config_.quote_listen_addresses() ) );
}

multicast_communication::mediator::~mediator()
{
}
//
void multicast_communication::mediator::cts_line_message( const common::buffer_ptr ptr, const size_t size )
{
	cts_parser_.process_message( ptr, size );
}
void multicast_communication::mediator::cqs_line_message( const common::buffer_ptr ptr, const size_t size )
{
	cqs_parser_.process_message( ptr, size );
}
