#ifndef _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_
#define _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>
#include <list>
#include <iomanip>

#include "msg_utility.h"

namespace multicast_communication
{
	class trade_message;
	typedef boost::shared_ptr< trade_message > trade_message_ptr;
	typedef std::list<trade_message_ptr> trade_message_list_ptr;

	class trade_message
	{
	public:
		enum trade_type
		{
			unknown_trade,
			short_trade,
			long_trade,
		};
	private:
		enum trade_signature
		{
			sig_short_trade = 'I',
			sig_long_trade = 'B',
		};

		enum
		{
			st_security_symbol_pos = 22,
			st_trade_volume_pos = 1,
			st_trade_end_pos = 3,
		};
		enum
		{
			st_security_symbol_len = 3,
			st_trade_volume_len = 4,
			st_price_denominator_indicator_len = 1,
			st_trade_price_len = 8,
		};

		enum
		{
			lt_security_symbol_pos = 22,
			lt_price_denominator_indicator_pos = 21,
			lt_trade_end_pos = 4,
		};
		enum
		{
			lt_security_symbol_len = 11,
			lt_price_denominator_indicator_len = 1,
			lt_trade_price_len = 12,
			lt_trade_volume_len = 9,
		};
		
		std::string security_symbol_;
		double price_;
		double volume_;
		trade_type type_;

		void parse_short_trade(std::istream &);
		void parse_long_trade(std::istream &);
	public:
		trade_message();
		bool parse_trade(std::istream &);
		static bool parse_block(const std::string &, trade_message_list_ptr &);
		std::string security_symbol() const;
		double price() const;
		double volume() const;
		trade_type type() const;
	};

	std::ostream& operator<<( std::ostream& output, trade_message& msg );
	
}

#endif // _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_
