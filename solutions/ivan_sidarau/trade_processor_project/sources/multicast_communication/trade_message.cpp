#include "trade_message.h"

#include <time_helper.h>

multicast_communication::trade_message::trade_message()
: message_time_( common::create_ptime( 0, 0, 0 ) )
, minute_( common::create_uint_time( 0, 0, 0 ) )
, price_( 0.0 )
, volume_( 0.0 )
{
}
multicast_communication::trade_message::trade_message( const cts_protocol::message_header& mh, const cts_protocol::short_trade& st )
{
	using namespace cts_protocol;

	const char* security_symbol_end = std::find( st.security_symbol, st.security_symbol + short_trade::security_symbol_size, end_of_string );
	security_symbol_ = std::string( st.security_symbol, security_symbol_end - st.security_symbol );

	minute_ = get_time( mh );
	message_time_ = common::convert( minute_ );
	price_ = get_price( st );
	volume_ = get_volume( st );
}
multicast_communication::trade_message::trade_message( const cts_protocol::message_header& mh, const cts_protocol::long_trade& lt )
{
	using namespace cts_protocol;

	const char* security_symbol_end = std::find( lt.security_symbol, lt.security_symbol + long_trade::security_symbol_size, end_of_string );
	security_symbol_ = std::string( lt.security_symbol, security_symbol_end - lt.security_symbol );

	minute_ = get_time( mh );
	message_time_ = common::convert( minute_ );
	price_ = get_price( lt );
	volume_ = get_volume( lt );
}
//
const boost::posix_time::ptime& multicast_communication::trade_message::message_time() const
{
	return message_time_;
}

boost::uint64_t multicast_communication::trade_message::minute() const
{
	return minute_;
}
const std::string& multicast_communication::trade_message::security_symbol() const
{
	return security_symbol_;
}

double multicast_communication::trade_message::price() const
{
	return price_;
}

double multicast_communication::trade_message::volume() const
{
	return volume_;
}
//
multicast_communication::trade_message_ptr multicast_communication::trade_message::create_test_message( 
	const boost::uint64_t minute, 
	const std::string security_symbol, 
	double price, 
	double volume )
{
	trade_message_ptr result( new trade_message() );
	result->message_time_ = common::convert( minute );
	result->minute_ = minute;
	result->security_symbol_ = security_symbol;
	result->price_ = price;
	result->volume_ = volume;
	return result;
}
//
std::ostream& multicast_communication::operator<<( std::ostream& out, const multicast_communication::trade_message& trade )
{
	out << "T " << trade.security_symbol().c_str() << " " << trade.price() << " " << trade.volume();
	return out;
}
