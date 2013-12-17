#include <ts_minute_storage.h>

minute_calculator::ts_minute_storage::ts_minute_storage()
{
}
minute_calculator::ts_minute_storage::~ts_minute_storage()
{
}

minute_calculator::stock_minute_data_ptr minute_calculator::ts_minute_storage::new_data( const multicast_communication::trade_message_ptr& trade )
{
	return new_data_( trade );
}
minute_calculator::stock_minute_data_ptr minute_calculator::ts_minute_storage::new_data( const multicast_communication::quote_message_ptr& quote )
{
	return new_data_( quote );
}

minute_calculator::stock_minute_data_ptr minute_calculator::ts_minute_storage::get_last_minute()
{
	boost::mutex::scoped_lock lock( protector_ );
	stock_minute_data_ptr result = data_;
	data_.reset();
	return result;
}
