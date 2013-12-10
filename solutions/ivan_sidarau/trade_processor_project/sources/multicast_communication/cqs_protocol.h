#ifndef _MULTICAST_COMMUNICATION_CQS_PROTOCOL_H_
#define _MULTICAST_COMMUNICATION_CQS_PROTOCOL_H_

#include <cmath>

namespace multicast_communication
{
	namespace cqs_protocol
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
		struct short_quote
		{
			static const size_t security_symbol_size = 3ul;
			static const size_t price_size = 8ul;
			static const size_t volume_size = 3ul;

			char security_symbol[ security_symbol_size ];
			char reserved[3];
			char bid_price_denominator_indicator[1];
			char bid_price[ price_size ];
			char bid_size_units_of_trade[ volume_size ];
			char reserved_2[1];
			char offer_price_denominator_indicator[1];
			char offer_price[ price_size ];
			char offer_size_units_of_trade[ volume_size ];
			char reserved_3[1];
			char national_bbo_indicator[1];
			char finra_bbo_indicator[1];
		};
		struct long_quote
		{
			static const size_t security_symbol_size = 11ul;
			static const size_t price_size = 12ul;
			static const size_t volume_size = 7ul;

			char security_symbol[ security_symbol_size ];
			char reserved[16];
			char bid_price_denominator_indicator[1];
			char bid_price[ price_size ];
			char bid_size_units_of_trade[ volume_size ];
			char offer_price_denominator_indicator[1];
			char offer_price[ price_size ];
			char offer_size_units_of_trade[ volume_size ];
			char reserved_3[9];
			char national_bbo_indicator[1];
			char finra_bbo_indicator[1];
		};

		static const size_t national_appendage_long_size = 58ul;
		static const size_t national_appendage_exp_short_size = 28ul;

		static const size_t finra_appendage_size = 56ul;

		template< typename quote_type >
		inline size_t sizeof_appendages( const quote_type& q )
		{
			size_t appendage_size = 0;
			if ( q.national_bbo_indicator[0] == '4' )
				appendage_size += national_appendage_long_size;
			else
			if ( q.national_bbo_indicator[0] == '6' )
				appendage_size += national_appendage_exp_short_size;
			if ( q.finra_bbo_indicator[0] == '3' )
				appendage_size += finra_appendage_size;
			return appendage_size;
		}
		//
		template< typename quote_type >
		inline double get_bid_price( const quote_type& q )
		{
			double price = 0.0;
			for (size_t i = 0 ; i < quote_type::price_size ; ++i )
				price = price * 10 + ( q.bid_price[ i ] - '0' );

			double denominator = static_cast< double >( q.bid_price_denominator_indicator[0] - 'A' + 1 );
			if ( q.bid_price_denominator_indicator[0] == 'I' )
				denominator = 0.0;

			price /= pow( 10.0, (double)denominator );
			return price;
		}

		template< typename quote_type >
		inline double get_bid_volume( const quote_type& q )
		{
			double volume = 0.0;
			for (size_t i = 0 ; i < quote_type::volume_size ; ++i )
				volume = volume * 10 + ( q.bid_size_units_of_trade[ i ] - '0' );
			return volume;
		}
		template< typename quote_type >
		//
		inline double get_offer_price( const quote_type& q )
		{
			double price = 0.0;
			for (size_t i = 0 ; i < quote_type::price_size ; ++i )
				price = price * 10 + ( q.offer_price[ i ] - '0' );

			double denominator = static_cast< double >( q.offer_price_denominator_indicator[0] - 'A' + 1 );
			if ( q.offer_price_denominator_indicator[0] == 'I' )
				denominator = 0.0;

			price /= pow( 10.0, (double)denominator );
			return price;
		}

		template< typename quote_type >
		inline double get_offer_volume( const quote_type& q )
		{
			double volume = 0.0;
			for (size_t i = 0 ; i < quote_type::volume_size ; ++i )
				volume = volume * 10 + ( q.offer_size_units_of_trade[ i ] - '0' );
			return volume;
		}
	}
}

#endif // _MULTICAST_COMMUNICATION_CQS_PROTOCOL_H_

