#include "trade_message.h"
#include <iomanip>
#include <string>

namespace multicast_communication {

static const char ST = 'I';
static const char LT = 'B';


const uint32_t short_trade_volume_left = 1;
const uint32_t short_trade_end_left = 3;

const uint32_t short_security_symbol_len = 3;
const uint32_t short_trade_volume_len = 4;
const uint32_t short_price_denominator_indicator_len = 1;
const uint32_t short_trade_price_len = 8;

            
const uint32_t long_price_denominator_indicator_left = 21;
const uint32_t long_trade_end_left = 4;

const uint32_t long_security_symbol_len = 11;
const uint32_t long_price_denominator_indicator_len = 1;
const uint32_t long_trade_price_len = 12;
const uint32_t long_trade_volume_len = 9;


trade_message::trade_message():type_(type_unknown),
    security_symbol_(""),
    price_(0.0),
    volume_(0.0)
{

}

trade_message::trade_message( std::istream& in, message_type type ):type_(type)
{
    switch( type )
    {
        case type_short:
        {
            security_symbol_ = get<std::string, short_security_symbol_len>( in );		
            in.seekg(short_trade_volume_left, std::istream::cur);
	
            volume_ = get< double, short_trade_volume_len>(in);

            double denominator = denominators.at( get< std::string, short_price_denominator_indicator_len >( in )[0] );
            price_ = get< double, short_trade_price_len>(in)/ denominator;	

            in.seekg( short_trade_end_left, std::istream::cur );
        }
        break;
    case type_long:
        {
	        security_symbol_ = get<std::string, long_security_symbol_len>(in);

	        in.seekg(long_price_denominator_indicator_left, std::istream::cur);

	        double denominator = denominators.at( get< std::string,  long_price_denominator_indicator_len >( in )[0] );
	        price_ = get<double, long_trade_price_len>(in)/ denominator;	
	
	        volume_ = get<double, long_trade_volume_len >(in);

	        in.seekg( long_trade_end_left, std::istream::cur );
        }
        break;
    }

}

std::string trade_message::security_symbol() const
{
	return security_symbol_;
}

double trade_message::price() const
{
	return price_;
}

double trade_message::volume() const
{
	return volume_;
}


template<>
trade_message_ptr build<trade_message_ptr>( std::istream& in )
{
    using namespace multicast_communication;
    char type;
	in.seekg(1, std::istream::cur);
	type = in.get();

	switch(in.get())
	{
    case ST:
        in.seekg(22, std::istream::cur);
        return trade_message_ptr( new trade_message(in, type_short));
    case LT:
        in.seekg(22, std::istream::cur);
		return trade_message_ptr( new trade_message(in, type_long));
	}
    return trade_message_ptr();
}

std::ostream& operator << ( std::ostream& out, const trade_message_ptr& msg )
{
      return out << std::fixed << "T " << msg->security_symbol() 
                               << " " << std::setprecision( 2 ) << msg->price() 
                               << " " << std::setprecision( 1 ) << msg->volume() 
                               << std::endl;
}

} //multicast_communication
