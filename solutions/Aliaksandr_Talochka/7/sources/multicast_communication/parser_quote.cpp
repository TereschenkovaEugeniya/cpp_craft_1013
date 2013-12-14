#include "parser_quote.h"


parser_messages::parser_quote::parser_quote()
{
}

parser_messages::parser_quote::~parser_quote()
{
}

std::vector<messages::quote_message> parser_messages::parser_quote::parse(const std::string &quote)
{

	std::vector<messages::quote_message> quote_vector;


	switch (quote.c_str()[2])
	{
		case parser_messages::parser_quote::short_message :
			quote_vector.push_back(parse_short(quote, 1));
			break;
		case  parser_messages::parser_quote::long_message :
			quote_vector.push_back(parse_long(quote, 1));
			break;
	}

	for(size_t i = 0; i<quote.size(); i++)
	{
		if(quote.c_str()[i] == 0x1F)
		{
			switch (quote.c_str()[i + 2])
			{
				case parser_messages::parser_quote::short_message :
					quote_vector.push_back(parse_short(quote, i+1));
					break;
				case  parser_messages::parser_quote::long_message :
					quote_vector.push_back(parse_long(quote, i+1));
					break;
			}
		}
	}

	return quote_vector;
}

messages::quote_message parser_messages::parser_quote::parse_short(const std::string &quote, const short shift)
{
	messages::ShortQuote* ptrShortQuote = (messages::ShortQuote*)(quote.c_str() + shift);
	messages::quote_message ret;

	ret.security_symbol = std::string(ptrShortQuote->SecuritySymbol, 3);
	ret.bid_price = boost::lexical_cast<double>(std::string(ptrShortQuote->BidShortPrice, 8));
	ret.bid_volume = boost::lexical_cast<double>(std::string(ptrShortQuote->BidSizeUnitsOfTrade,3));
	ret.offer_price = boost::lexical_cast<double>(std::string(ptrShortQuote->OfferShortPrice,8));
	ret.offer_volume = boost::lexical_cast<double>(std::string(ptrShortQuote->OfferSizeUnitsOfTrade,3));
	ret.time = convert_time(ptrShortQuote->Header[18])*3600 + convert_time(ptrShortQuote->Header[19])*60 + convert_time(ptrShortQuote->Header[20]);

	

	return ret;
}

messages::quote_message parser_messages::parser_quote::parse_long(const std::string &quote, const short shift)
{
	messages::LongQuote* ptrLongQuote = (messages::LongQuote*)(quote.c_str() + shift);
	messages::quote_message ret;

	ret.security_symbol = std::string(ptrLongQuote->SecuritySymbol, 11);
	ret.bid_price = boost::lexical_cast<double>(std::string(ptrLongQuote->BidPrice_LLPB, 12));
	ret.bid_volume = boost::lexical_cast<double>(std::string(ptrLongQuote->BidSize, 7));
	ret.offer_price = boost::lexical_cast<double>(std::string(ptrLongQuote->OfferPrice_LLPB, 12));
	ret.offer_volume = boost::lexical_cast<double>(std::string(ptrLongQuote->OfferSize, 7));
	ret.time = convert_time(ptrLongQuote->Header[18])*3600 + convert_time(ptrLongQuote->Header[19])*60 + convert_time(ptrLongQuote->Header[20]);

	return ret;
}