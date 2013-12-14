#ifndef _PARSER_QUOTE_H_
#define _PARSER_QUOTE_H_

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
		unsigned int convert_time (const unsigned char symbol)
		{
			if((symbol >= '0')&&(symbol <= 'k'))
				return symbol-48;
			else
				return 0;
		}

		enum signature_message
		{
			short_message = 'D',
			long_message = 'B',
		};
	};
}

#endif