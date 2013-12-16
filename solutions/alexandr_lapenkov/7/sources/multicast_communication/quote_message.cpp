#include "quote_message.h"
#include <boost/lexical_cast.hpp>

namespace multicast_communication
{
	quote_message::quote_message( boost::function< void( const quote_message_ptr& ) > handler )
		: handler( handler )
	{}

	std::string quote_message::get_security_symbol() const
	{
		std::string result;
		for( size_t i = 0; i < security_symbol.length() && security_symbol[i] != ' '; ++i )
			result += security_symbol[i];
		return result;
	}

	double quote_message::get_bid_price() const
	{
		return boost::lexical_cast< double >( bid_price );
	}

	double quote_message::get_bid_volume() const
	{
		return boost::lexical_cast< double >( bid_size_in_units_of_trade );
	}

	double quote_message::get_offer_price() const
	{
		return boost::lexical_cast< double >( offer_price );
	}

	double quote_message::get_offer_volume() const
	{
		return boost::lexical_cast< double >( offer_size_in_units_of_trade );
	}

	unsigned quote_message::get_minute() const
	{
		return ( time[0] - 48 ) * 60 + ( time[1] - 48 ) ;
	}

	void quote_message::parse( binreader& in, const char category, const char type )
	{
		in.skip( 16 );
		time = in.get_line( 6 );
		if( ( type == 'D' && ( category == 'E' || category == 'L' ) ) )
		{
			security_symbol = in.get_line( 3 );
			in.skip( 3 );
			bid_price_denominator_indicator = in.get();
			bid_price = in.get_line( 8 );
			bid_size_in_units_of_trade = in.get_line( 3 );
			in.skip( 1 );
			offer_price_denominator_indicator = in.get();
			offer_price = in.get_line( 8 );
			offer_size_in_units_of_trade = in.get_line( 3 );
			in.skip( 3 );
			handler( shared_from_this() );
		}
		else
		if( ( type == 'B' && ( category == 'B' || category == 'E' || category == 'L' ) ) )
		{
			security_symbol = in.get_line( 11 );
			in.skip( 16 );
			lower_limit_price_band_denominator_indicator = in.get();
			bid_price = in.get_line( 12 );
			bid_size_in_units_of_trade = in.get_line( 7 );
			upper_limit_price_band_denominator_indicator = in.get();
			offer_price = in.get_line( 12 );
			offer_size_in_units_of_trade = in.get_line( 7 );
			in.skip( 11 );
			handler( shared_from_this() );
		}
		else
			return ;
	}

}
