#include <stock_minute_storage.h>


minute_calculator::stock_minute_storage::stock_minute_storage( const std::string& filepath )
: filepath_( filepath )
{
}
minute_calculator::stock_minute_storage::~stock_minute_storage()
{
	boost::mutex::scoped_lock lock( map_mutex_ );
	for( minutes::iterator i = minutes_.begin() ; i != minutes_.end() ; ++i )
	{
		const stock_minute_data_ptr last_minute = i->second->get_last_minute();
		print_data_( i->first, last_minute );
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
void minute_calculator::stock_minute_storage::print_data_( const std::string& name, const stock_minute_data_ptr& data )
{
	if ( data )
	{
		std::ofstream output( ( filepath_ + "/" + name + ".dat").c_str(), std::ios::app | std::ios::binary );
		if ( output.is_open() )
		{
			data->print_binary( output );
			output.close();
		}
		else
			; // think about processing names that cannot be used as filename on filesystem
	}
}

