#include "quote_message.h"
#include <iomanip>
#include <string>

#include "base.h"

namespace multicast_communication {

static const char SQ = 'D';
static const char LQ = 'B';

const uint32_t short_bid_price_denominator_indicator_left = 3;
const uint32_t short_offer_price_denominator_indicator_left = 1;
const uint32_t short_quote_end_left = 3;
            
const uint32_t short_security_symbol_len = 3;
const uint32_t short_denominato_len = 1;
const uint32_t short_price_len = 8;
const uint32_t short_volume_len = 3;
            
const uint32_t long_bid_price_denominator_indicator_left = 16;
const uint32_t long_offer_price_denominator_indicator_left = 0;
const uint32_t long_quote_end_left = 11;
            
const uint32_t long_security_symbol_len = 11;
const uint32_t long_denominato_len = 1;
const uint32_t long_price_len = 12;
const uint32_t long_volume_len = 7;


quote_message::quote_message():type_(type_unknown),
        security_symbol_(""),      
        bid_price_(0),
        bid_volume_(0),        
        offer_price_(0), 
        offer_volume_(0)
{
}
quote_message::quote_message(std::istream& in, message_type type):type_(type)
{
    switch (type_)
    {
    case type_short:
        {
            security_symbol_ = get<std::string, short_security_symbol_len>(in);
		
	        in.seekg(short_bid_price_denominator_indicator_left, std::istream::cur);
		
	        double bid_price_denominator = denominators.at(get<std::string, short_denominato_len>(in)[0]);
	        bid_price_ = get<double, short_price_len>(in)/ bid_price_denominator;
	        bid_volume_ = get<double, short_volume_len>(in);

	        in.seekg(short_offer_price_denominator_indicator_left, std::istream::cur);
		
	        double offer_price_denominator = denominators.at(get<std::string, short_denominato_len>(in)[0]);
	        offer_price_ = get<double, short_price_len>(in)/ offer_price_denominator;
	        offer_volume_ = get<double, short_volume_len>(in);

	        in.seekg(short_quote_end_left, std::istream::cur);            
        }
        break;
    case type_long:
        {
            security_symbol_ = get<std::string, long_security_symbol_len>(in);
		
	        in.seekg(long_bid_price_denominator_indicator_left, std::istream::cur);
		
	        double bid_price_denominator = denominators.at(get<std::string, long_denominato_len>(in)[0]);
	        bid_price_ = get<double, long_price_len>(in)/ bid_price_denominator;
	        bid_volume_ = get<double, long_volume_len>(in);

	        in.seekg(long_offer_price_denominator_indicator_left, std::istream::cur);
		
	        double offer_price_denominator = denominators.at(get<std::string, long_denominato_len>(in)[0]);
	        offer_price_ = get<double, long_price_len>(in)/ offer_price_denominator;
	        offer_volume_ = get<double, long_volume_len>(in);

	        in.seekg(long_quote_end_left, std::istream::cur);
        }
        break;
    }
}

template<>
quote_message_ptr build<quote_message_ptr>( std::istream& in )
{
    using namespace multicast_communication;
    char type;
	in.seekg(1, std::istream::cur);
	type = in.get();

	switch(in.get())
	{
    case SQ:
        in.seekg(22, std::istream::cur);
        return quote_message_ptr( new quote_message(in, type_short));
    case LQ:
        in.seekg(22, std::istream::cur);
		return quote_message_ptr( new quote_message(in, type_long));
	}
    return quote_message_ptr();
}

std::string quote_message::security_symbol( ) const
{
	return security_symbol_;
}

double quote_message::bid_price( ) const
{
	return bid_price_;
}

double quote_message::bid_volume( ) const
{
	return bid_volume_;
}

double quote_message::offer_price( ) const
{
	return offer_price_;
}

double quote_message::offer_volume( ) const
{
	return offer_volume_;
}


std::ostream& multicast_communication::operator << ( std::ostream& out, const quote_message_ptr& msg )
{
    return out << std::fixed << "Q " << msg->security_symbol() 
                             << " " << std::setprecision( 2 ) << msg->bid_price() 
                             << " " << std::setprecision( 1 ) << msg->bid_volume() 
                             << " " << std::setprecision( 2 ) << msg->offer_price() 
                             << " " << std::setprecision( 1 ) << msg->offer_volume() 
                             << std::endl;    
}


} //namespace multicast_communication 