#pragma once

#include "messages.h"
#include <vector>
#include <algorithm>
#include <math.h>
#include <boost/lexical_cast.hpp>

namespace parser_messages
{
	class parser_quote 
	{
	public:
		parser_quote();
		virtual ~parser_quote();

		std::vector<messages::quote_message> parse(const std::string &quote);
	
	protected:
		messages::quote_message parse_short(const std::string &quote, const short shift);
		messages::quote_message parse_long(const std::string &quote, const short shift);

		enum signature_message
		{
			short_message = 'D',
			long_message = 'B',
		};

		/*enum format_short_quote
		{
		message_header = 24,
		security_symbol = 3, 
		quote_condition = 1,
		limitup_limit_down_indicator = 1,
		reserved = 1,
		bid_price_enominator Indicator 1 Bid Short Price 8 Bid Size Units of Trade 3 Reserved 1 Offer Price Denominator Indicator 1 Offer Short Price 8 Offer Size in Units of Trade 3 Reserved 1 National BBO Indicator 1 FINRA BBO Indicator 1
		};*/
	};
}