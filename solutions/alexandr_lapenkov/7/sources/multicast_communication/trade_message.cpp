#include "trade_message.h"
#include <boost/lexical_cast.hpp>

namespace multicast_communication
{
	trade_message::trade_message( boost::function< void( const trade_message_ptr& ) > handler )
		: handler( handler )
	{}

	std::string trade_message::get_security_symbol() const
	{
		std::string result;
		for( size_t i = 0; i < security_symbol.length() && security_symbol[i] !=' '; ++i )
			result += security_symbol[i];
		return result;
	}

	double trade_message::get_price() const
	{
		return boost::lexical_cast< double >( trade_price );
	}

	double trade_message::get_volume() const
	{
		return boost::lexical_cast< double >( trade_volume );
	}

	unsigned trade_message::get_minute() const
	{
		return ( time[0] - 48 ) * 60 + ( time[1] - 48 ) ;
	}

	void trade_message::parse( binreader& in, const char category, const char type )
	{
		in.skip( 16 );
		time = in.get_line( 6 );
		if( type == 'I' && ( category == 'E' || category == 'L' ) )
		{
			security_symbol = in.get_line( 3 );
			in.skip( 1 );
			trade_volume = in.get_line( 4 );
			price_denominator_indicator = in.get();
			trade_price = in.get_line( 8 );
			in.skip( 3 );
			handler( shared_from_this() );
		}
		else
		if( type == 'B' && ( category == 'B' || category == 'E' || category == 'L' ) )
		{
			security_symbol = in.get_line( 11 );
			in.skip( 21 );
			price_denominator_indicator = in.get();
			trade_price = in.get_line( 12 );
			trade_volume = in.get_line( 9 );
			in.skip( 4 );
			handler( shared_from_this() );
		}
		else
			return ;
	}

}
