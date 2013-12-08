#include "trade_message.h"

multicast_communication::trade_message::trade_message() : security_symbol_(""), price_(0), volume_(0)
{
}

std::string multicast_communication::trade_message::security_symbol() const
{
	return security_symbol_;
}

double multicast_communication::trade_message::price() const
{
	return price_;
}

double multicast_communication::trade_message::volume() const
{
	return volume_;
}

bool multicast_communication::trade_message::initialize(const std::string& msg)
{
	if (msg.length() < 44)
	{
		return false;
	}

	std::string header = msg.substr(0, 24);
	std::string body = msg.substr(24, msg.length() - 24);

	if ((header[0] == 'E' || header[0] == 'L') && header[1] == 'I')
	{
		return init_short(body);
	}
	else if ((header[0] == 'B' || header[0] == 'E' || header[0] == 'L') && header[1] == 'B')
	{
		return init_long(body);
	}

	return false;
}

bool multicast_communication::trade_message::init_short(const std::string& body)
{
	if (body.length() < 20)
	{
		return false;
	}

	security_symbol_ = body.substr(0, 3);

	std::string trade_volume = body.substr(4, 4);
	double denominator = body[8];
	std::string price = body.substr(9, 8);

	price_ = boost::lexical_cast<double>(price)/denominator;
	volume_ = boost::lexical_cast<double>(trade_volume);

	return true;
}

bool multicast_communication::trade_message::init_long(const std::string& body)
{
	if (body.length() < 58)
	{
		return false;
	}

	security_symbol_ = body.substr(0, 11);
		
	double denominator = body[32];
	std::string price = body.substr(33, 12);
	std::string trade_volume = body.substr(45, 9);

	price_ = boost::lexical_cast<double>(price)/denominator;
	volume_ = boost::lexical_cast<double>(trade_volume);
}