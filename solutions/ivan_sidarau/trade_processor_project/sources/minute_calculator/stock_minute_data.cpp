#include <stock_minute_data.h>

const char minute_calculator::stock_minute_data::empty[ name_length ] = {0};

minute_calculator::stock_minute_data::stock_minute_data( const multicast_communication::trade_message_ptr& trade )
: minute_time_( trade->message_time() )
, open_minute_( trade->minute() )
, close_minute_( trade->minute() )
, name_( trade->security_symbol() )
, open_price_( trade->price() )
, high_price_( trade->price() )
, low_price_( trade->price() )
, close_price_( trade->price() )
, volume_( trade->volume() )
, bid_( 0.0 )
, ask_( 0.0 )
{
}
minute_calculator::stock_minute_data::stock_minute_data( const multicast_communication::quote_message_ptr& quote )
: minute_time_( quote->message_time() )
, open_minute_( quote->minute() )
, close_minute_( quote->minute() )
, name_( quote->security_symbol() )
, open_price_( 0.0 )
, high_price_( 0.0 )
, low_price_( 0.0 )
, close_price_( 0.0 )
, volume_( 0.0 )
, bid_( quote->bid_volume() )
, ask_( quote->offer_volume() )
{
}
//
const boost::posix_time::ptime& minute_calculator::stock_minute_data::minute_time() const
{
	return minute_time_;
}
//
void minute_calculator::stock_minute_data::add_market_data( const multicast_communication::trade_message_ptr& trade )
{
	const boost::uint64_t minute = trade->minute();
	const double new_price = trade->price();
	if ( minute < open_minute_ )
	{
		open_minute_ = minute;
		open_price_ = new_price;
	}
	if ( new_price > high_price_ )
		high_price_ = new_price;
	if ( new_price < low_price_ )
		low_price_ = new_price;
	if ( minute > close_minute_ )
	{
		close_minute_ = minute;
		close_price_ = new_price;
	}

	volume_ += trade->volume();
}
void minute_calculator::stock_minute_data::add_market_data( const multicast_communication::quote_message_ptr& quote )
{
	bid_ += quote->bid_volume();
	ask_ += quote->offer_volume();
}
//
std::ostream& minute_calculator::operator<<( std::ostream& out, const stock_minute_data& sm )
{
	out << sm.open_minute_ << " " << sm.name_.c_str() << " (" << sm.open_price_ << " " << sm.high_price_ << " " << 
		sm.low_price_ << " " << sm.close_price_ << ") " << sm.volume_ << " (" << sm.bid_ << " " << sm.ask_ << ")";
	return out;
}

void minute_calculator::stock_minute_data::print_binary( std::ostream& out )
{
	out.write( reinterpret_cast< char* >( &open_minute_ ), sizeof( open_minute_ ) );
	
	out.write( name_.c_str(), name_.size() );
	out.write( empty, name_length - name_.size() );
	
	out.write( reinterpret_cast< char* >( &open_price_ ), sizeof( open_price_ ) );
	out.write( reinterpret_cast< char* >( &high_price_ ), sizeof( high_price_ ) );
	out.write( reinterpret_cast< char* >( &low_price_ ), sizeof( low_price_ ) );
	out.write( reinterpret_cast< char* >( &close_price_ ), sizeof( close_price_ ) );

	out.write( reinterpret_cast< char* >( &volume_ ), sizeof( volume_ ) );
	out.write( reinterpret_cast< char* >( &bid_ ), sizeof( bid_ ) );
	out.write( reinterpret_cast< char* >( &ask_ ), sizeof( ask_ ) );
}
