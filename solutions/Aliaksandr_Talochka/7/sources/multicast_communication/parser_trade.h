#ifndef _PARSER_TRADE_H_
#define _PARSER_TRADE_H_

#include "messages.h"
#include <vector>
#include <algorithm>
#include <math.h>
#include <boost/lexical_cast.hpp>

namespace parser_messages
{
	class parser_trade 
	{
	public:
		parser_trade();
		virtual ~parser_trade();

		std::vector<messages::trade_message> parse(const std::string &trade);
	
	protected:
		messages::trade_message parse_short(const std::string &trade, const short shift);
		messages::trade_message parse_long(const std::string &trade, const short shift);
		unsigned int convert_time (const unsigned char symbol)
		{
			if((symbol >= '0')&&(symbol <= 'k'))
				return symbol-48;
			else
				return 0;
		}

		enum signature_message
		{
			short_message = 'I',
			long_message = 'B',
		};
	};
}

#endif  //_PARSER_TRADE_H_