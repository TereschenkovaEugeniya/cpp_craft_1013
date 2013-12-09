#include <processors_test_helper.h>

void multicast_communication::tests_::market_data_receiver_manager_test_helper::cts_line_message( 
	const common::buffer_ptr ptr, const size_t size )
{
	boost::mutex::scoped_lock lock( cts_message_sizes_protector_ );
	cts_message_sizes_.push_back( size );
}
void multicast_communication::tests_::market_data_receiver_manager_test_helper::cqs_line_message( 
	const common::buffer_ptr ptr, const size_t size )
{
	boost::mutex::scoped_lock lock( cqs_message_sizes_protector_ );
	cqs_message_sizes_.push_back( size );
}

//
void multicast_communication::tests_::market_data_processor_test_helper::new_trade( const trade_message_ptr& )
{
}
void multicast_communication::tests_::market_data_processor_test_helper::new_quote( const quote_message_ptr& )
{
}
