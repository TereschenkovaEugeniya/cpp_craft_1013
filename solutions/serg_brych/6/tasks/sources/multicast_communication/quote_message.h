#ifndef _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_
#define _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>

#include <list>
#include <iomanip>

#include "msg_utility.h"

namespace multicast_communication
{
	class quote_message;
	typedef boost::shared_ptr< quote_message > quote_message_ptr;
	typedef std::list<quote_message_ptr> quote_message_list_ptr;

	class quote_message
	{
	public:
		enum quote_type
		{
			unknown_quote,
			short_quote,
			long_quote,
		};
	private:
		enum
		{
			sq_security_symbol_pos = 22,
			sq_bid_price_denominator_indicator_pos = 3,
			sq_offer_price_denominator_indicator_pos = 1,
			sq_short_quote_end_pos = 3,
		};
		enum
		{
			sq_security_symbol_len = 3,
			sq_denominato_len = 1,
			sq_price_len = 8,
			sq_volume_len = 3,
		};

		enum 
		{
			lq_security_symbol_pos = 22,
			lq_bid_price_denominator_indicator_pos = 16,
			lq_offer_price_denominator_indicator_pos = 0,
			lq_short_quote_end_pos = 11,
		};
		enum 
		{
			lq_security_symbol_len = 11,
			lq_denominato_len = 1,
			lq_price_len = 12,
			lq_volume_len = 7,
		};

		enum quote_signature
		{
			sig_short_quote = 'D',
			sig_long_quote = 'B',
		};
	
		std::string security_symbol_;
		double bid_price_;
		double bid_volume_;
		double offer_price_;
		double offer_volume_;
		void parse_short_quote(std::istream &);
		void parse_long_quote(std::istream &);
		quote_type type_;
	public:
		
		quote_message();
		bool parse_quote(std::istream &);
		std::string security_symbol() const;
		static bool parse_block(const std::string &, quote_message_list_ptr &);
		double bid_price() const;
		double bid_volume() const;
		double offer_price() const;
		double offer_volume() const;
		quote_type type() const;
	};

	std::ostream& operator<<( std::ostream& output, quote_message& msg );
}

#endif // _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_
