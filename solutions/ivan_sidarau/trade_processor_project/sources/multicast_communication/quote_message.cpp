#include "quote_message.h"

multicast_communication::quote_message::quote_message()
: bid_price_( 0.0 )
, bid_volume_( 0.0 )
, offer_price_( 0.0 )
, offer_volume_( 0.0 )
{
}
multicast_communication::quote_message::quote_message( const cqs_protocol::message_header& mh, const cqs_protocol::short_quote& sq )
{
	using namespace cqs_protocol;

	const char* security_symbol_end = std::find( sq.security_symbol, sq.security_symbol + short_quote::security_symbol_size, end_of_string );
	security_symbol_ = std::string( sq.security_symbol, security_symbol_end - sq.security_symbol );

	bid_price_ = get_bid_price( sq );
	bid_volume_ = get_bid_volume( sq );
	offer_price_ = get_offer_price( sq );
	offer_volume_ = get_offer_volume( sq );
}
multicast_communication::quote_message::quote_message( const cqs_protocol::message_header& mh, const cqs_protocol::long_quote& lq )
{
	using namespace cqs_protocol;

	const char* security_symbol_end = std::find( lq.security_symbol, lq.security_symbol + long_quote::security_symbol_size, end_of_string );
	security_symbol_ = std::string( lq.security_symbol, security_symbol_end - lq.security_symbol );

	bid_price_ = get_bid_price( lq );
	bid_volume_ = get_bid_volume( lq );
	offer_price_ = get_offer_price( lq );
	offer_volume_ = get_offer_volume( lq );
}

const std::string& multicast_communication::quote_message::security_symbol() const
{
	return security_symbol_;
}

double multicast_communication::quote_message::bid_price() const
{
	return bid_price_;
}

double multicast_communication::quote_message::bid_volume() const
{
	return bid_volume_;
}

double multicast_communication::quote_message::offer_price() const
{
	return offer_price_;
}

double multicast_communication::quote_message::offer_volume() const
{
	return offer_volume_;
}
