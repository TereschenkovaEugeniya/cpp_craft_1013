#ifndef _MULTICAST_COMMUNICATION_CTS_PROTOCOL_H_
#define _MULTICAST_COMMUNICATION_CTS_PROTOCOL_H_

#include <cmath>

#include <boost/cstdint.hpp>

#include <time_helper.h>

namespace multicast_communication
{
	namespace cts_protocol
	{
		static const char end_of_string = (char)32;

		struct message_header
		{
			char message_category[ 1 ];
			char message_type[ 1 ];
			char message_network[ 1 ];
			char retransmission_request[ 2 ];
			char header_identifier[ 1 ];
			char reserved[ 2 ];
			char message_sequence_number[ 9 ];
			char participant_id[ 1 ];
			char time_stamp[ 6 ];
		};
		struct short_trade
		{
			static const size_t security_symbol_size = 3ul;
			static const size_t trade_price_size = 8ul;
			static const size_t trade_volume_size = 4ul;

			char security_symbol[ security_symbol_size ];
			char sale_condition[1];
			char trade_volume[ trade_volume_size ];
			char price_denominator_indicator[1];
			char trade_price[ trade_price_size ];
			char consolidated_hll_indicator[1];
			char participant_ohll_indicator[1];
			char reserved[1];
		};
		struct long_trade
		{
			static const size_t security_symbol_size = 11ul;
			static const size_t trade_price_size = 12ul;
			static const size_t trade_volume_size = 9ul;

			char security_symbol[ security_symbol_size ];
			char reserved[6];
			char currency_indicator[3];
			char held_trade_indicator[1];
			char instrument_type[1];
			char reserved_2[10];
			char price_denominator_indicator[1];
			char trade_price[ trade_price_size ];
			char trade_volume[ trade_volume_size ];
			char reserved_3[4];
		};

		inline boost::uint64_t get_time( const message_header& mh )
		{
			const long hours = mh.time_stamp[0] - '0';
			const long minutes = mh.time_stamp[1] - '0';
			const long seconds = mh.time_stamp[2] - '0';
			const long milli = (mh.time_stamp[3] - '0') * 100 + (mh.time_stamp[4] - '0') * 10 + (mh.time_stamp[5] - '0');
			return common::create_uint_time( hours, minutes, seconds, milli );
		}

		inline double get_price( const short_trade& st )
		{
			double price = 0.0;
			for (size_t i = 0 ; i < short_trade::trade_price_size ; ++i )
				price = price * 10 + ( st.trade_price[ i ] - '0' );

			double denominator = static_cast< double >( st.price_denominator_indicator[0] - 'A' + 1 );
			if ( st.price_denominator_indicator[0] == 'I' )
				denominator = 0.0;

			price /= pow( 10.0, (double)denominator );

			return price;

		}
		inline double get_price( const long_trade& lt )
		{
			double price = 0.0;
			for (size_t i = 0 ; i < long_trade::trade_price_size ; ++i )
				price = price * 10 + ( lt.trade_price[ i ] - '0' );

			double denominator = static_cast< double >( lt.price_denominator_indicator[0] - 'A' + 1 );
			if ( lt.price_denominator_indicator[0] == 'I' )
				denominator = 0.0;

			price /= pow( 10.0, (double)denominator );

			return price;
		}

		inline double get_volume( const short_trade& st)
		{
			double volume = 0.0;
			for (size_t i = 0 ; i < short_trade::trade_volume_size ; ++i )
				volume = volume * 10 + ( st.trade_volume[ i ] - '0' );
			return volume;
		}
		inline double get_volume( const long_trade& lt)
		{
			double volume = 0.0;
			for (size_t i = 0 ; i < long_trade::trade_volume_size ; ++i )
				volume = volume * 10 + ( lt.trade_volume[ i ] - '0' );
			return volume;
		}

	}
}

#endif // _MULTICAST_COMMUNICATION_CTS_PROTOCOL_H_

