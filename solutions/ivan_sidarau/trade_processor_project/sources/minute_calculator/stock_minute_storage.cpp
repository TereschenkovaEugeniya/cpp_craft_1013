#include <stock_minute_storage.h>


minute_calculator::stock_minute_storage::stock_minute_storage( const std::string& filename )
: output_( filename.c_str(), std::ios::out | std::ios::binary )
{
}
minute_calculator::stock_minute_storage::~stock_minute_storage()
{
	boost::mutex::scoped_lock lock( map_mutex_ );
	for( minutes::iterator i = minutes_.begin() ; i != minutes_.end() ; ++i )
	{
		stock_minute_data_ptr last_minute = i->second->get_last_minute();
		if ( last_minute )
			last_minute->print_binary( output_ );
	}
}
void minute_calculator::stock_minute_storage::new_trade( multicast_communication::trade_message_ptr& trade )
{
	new_data_( trade );
}
void minute_calculator::stock_minute_storage::new_quote( multicast_communication::quote_message_ptr& quote )
{
	new_data_( quote );
}
