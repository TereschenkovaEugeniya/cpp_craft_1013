#include "parser_trade.h"


parser_messages::parser_trade::parser_trade()
{
}

parser_messages::parser_trade::~parser_trade()
{
}

std::vector<messages::trade_message> parser_messages::parser_trade::parse(const std::string &trade)
{

	std::vector<messages::trade_message> trade_vector;


	switch (trade.c_str()[2])
	{
		case parser_messages::parser_trade::short_message :
			trade_vector.push_back(parse_short(trade, 1));
			break;
		case  parser_messages::parser_trade::long_message :
			trade_vector.push_back(parse_long(trade, 1));
			break;
	}

	for(size_t i = 0; i < trade.size(); i++)
	{
		if(trade.c_str()[i] == 0x1F)
		{
			switch (trade.c_str()[i + 2])
			{
				case parser_messages::parser_trade::short_message :
					trade_vector.push_back(parse_short(trade, i+1));
					break;
				case  parser_messages::parser_trade::long_message :
					trade_vector.push_back(parse_long(trade, i+1));
					break;
			}
		}
	}

	return trade_vector;
}

messages::trade_message parser_messages::parser_trade::parse_short(const std::string &trade, const short shift)
{
	messages::ShortTrade* ptrShortTrade = (messages::ShortTrade*)(trade.c_str() + shift);
	messages::trade_message ret;

	ret.security_symbol = std::string(ptrShortTrade->SEC_Symbol, 3);
	ret.price = boost::lexical_cast<double>(std::string(ptrShortTrade->Trade_Price, 8));
	ret.volume = boost::lexical_cast<double>(std::string(ptrShortTrade->Trade_Volume, 4));
	ret.time = convert_time(ptrShortTrade->Msg_Header[18])*3600 + convert_time(ptrShortTrade->Msg_Header[19])*60 + convert_time(ptrShortTrade->Msg_Header[20]);


	return ret;
}

messages::trade_message parser_messages::parser_trade::parse_long(const std::string &trade, const short shift)
{
	messages::LongTrade* ptrLongTrade = (messages::LongTrade*)(trade.c_str() + shift);
	messages::trade_message ret;

	ret.security_symbol = std::string(ptrLongTrade->SEC_Symbol, 11);
	ret.price = boost::lexical_cast<double>(std::string(ptrLongTrade->Trade_Price, 12));
	ret.volume = boost::lexical_cast<double>(std::string(ptrLongTrade->Trade_Vol, 9));
	ret.time = convert_time(ptrLongTrade->Msg_Header[18])*3600 + convert_time(ptrLongTrade->Msg_Header[19])*60 + convert_time(ptrLongTrade->Msg_Header[20]);


	return ret;
}