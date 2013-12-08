#include "quote_message.h"


multicast_communication::quote_message::quote_message() : security_symbol_(""),
															bid_price_(0),
															bid_volume_(0),
															offer_price_(0),
															offer_volume_(0)
{
}

std::string multicast_communication::quote_message::security_symbol() const
{
	return security_symbol_;
}

double multicast_communication::quote_message::bid_price() const
{
	return bid_price_;
}

double multicast_communication::quote_message::bid_volume() const
{
	return bid_volume_;
}

double multicast_communication::quote_message::offer_price() const
{
	return offer_price_;
}

double multicast_communication::quote_message::offer_volume() const
{
	return offer_volume_;
}

bool multicast_communication::quote_message::initialize(const std::string& msg)
{
	if (msg.length() < 58)
	{
		return false;
	}

	std::string header = msg.substr(0, 24);
	std::string body = msg.substr(24, msg.length() - 24);

	if ((header[0] == 'E' || header[0] == 'L') && header[1] == 'D')
	{
		return init_short(body);
	}
	else if ((header[0] == 'B' || header[0] == 'E' || header[0] == 'L') && header[1] == 'B')
	{
		return init_long(body);
	}

	return false;
}

bool multicast_communication::quote_message::init_short(const std::string& body)
{
	int size = body.length();

	if (body.length() < 34)
	{
		return false;
	}

	security_symbol_ = body.substr(0, 3);

	double bid_denominator = body[6];
	std::string bid_short_price = body.substr(7, 8);
	std::string bid_volume = body.substr(15, 3);
	
	double offer_denominator = body[19];
	std::string offer_price = body.substr(20, 8);
	std::string offer_volume = body.substr(28, 3);
	
	bid_price_ = boost::lexical_cast<double>(bid_short_price)/bid_denominator;
	bid_volume_ = boost::lexical_cast<double>(bid_volume);

	offer_price_ = boost::lexical_cast<double>(offer_price)/offer_denominator;
	offer_volume_ = boost::lexical_cast<double>(offer_volume);

	return true;
}

bool multicast_communication::quote_message::init_long(const std::string& body)
{
	if (body.length() < 78)
	{
		return false;
	}

	security_symbol_ = body.substr(0, 11);

	double bid_denominator = body[27];
	std::string bid_short_price = body.substr(28, 12);
	std::string bid_volume = body.substr(40, 7);
	
	double offer_denominator = body[47];
	std::string offer_price = body.substr(48, 12);
	std::string offer_volume = body.substr(60, 7);
	
	bid_price_ = boost::lexical_cast<double>(bid_short_price)/bid_denominator;
	bid_volume_ = boost::lexical_cast<double>(bid_volume);

	offer_price_ = boost::lexical_cast<double>(offer_price)/offer_denominator;
	offer_volume_ = boost::lexical_cast<double>(offer_volume);

	return true;
}