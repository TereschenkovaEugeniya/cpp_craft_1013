#include "quote_message.h"


multicast_communication::quote_message::quote_message() : security_symbol_(""),
															bid_price_(0),
															bid_volume_(0),
															offer_price_(0),
															offer_volume_(0)
{
	denominators_ = boost::assign::map_list_of( '3' , 8.0 ) ( '4' , 16.0 ) ( '5' , 32.0 ) ( '6' , 64.0 ) ( '7' , 128.0 ) ( '8' , 256.0 )
		( 'A' , 10.0 ) ( 'B' , pow( 10.0, 2 ) ) ( 'C' , pow( 10.0, 3 ) ) ( 'D' , pow( 10.0, 4 ) ) ( 'E' , pow( 10.0, 5 ) ) ( 'F' , pow( 10.0, 6 ) ) ( 'G' , pow( 10.0, 7 ) ) ( 'H' , pow( 10.0, 8 ) );

	uint32_t time = 0;
	for (char ch = 48 ; ch < 108; ++ch, ++time)
	{
		time_stamp_[ch] = time;
	}
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

uint32_t multicast_communication::quote_message::time() const
{
	return time_;
}

bool multicast_communication::quote_message::initialize(const std::string& msg)
{
	if (msg.length() < 58)
	{
		return false;
	}

	std::string header = msg.substr(0, 24);
	std::string body = msg.substr(24, msg.length() - 24);

	std::string time = header.substr(18, 3);
	time_ = 3600 * time_stamp_[time[0]] + 60 * time_stamp_[time[1]] + time_stamp_[time[2]];

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

	double bid_denominator = 1;
	try
	{
		bid_denominator = denominators_.at(body[6]);
	}
	catch(std::out_of_range)
	{
	}
	std::string bid_short_price = body.substr(7, 8);
	std::string bid_volume = body.substr(15, 3);
	
	double offer_denominator = 1;
	try
	{
		offer_denominator = denominators_.at(body[19]);
	}
	catch(std::out_of_range)
	{
	}
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

	double bid_denominator = 1;
	try
	{
		bid_denominator = denominators_.at(body[27]);
	}
	catch(std::out_of_range)
	{
	}
	std::string bid_short_price = body.substr(28, 12);
	std::string bid_volume = body.substr(40, 7);
	
	double offer_denominator = 1;
	try
	{
		offer_denominator = denominators_.at(body[47]);
	}
	catch(std::out_of_range)
	{
	}
	std::string offer_price = body.substr(48, 12);
	std::string offer_volume = body.substr(60, 7);
	
	bid_price_ = boost::lexical_cast<double>(bid_short_price)/bid_denominator;
	bid_volume_ = boost::lexical_cast<double>(bid_volume);

	offer_price_ = boost::lexical_cast<double>(offer_price)/offer_denominator;
	offer_volume_ = boost::lexical_cast<double>(offer_volume);

	return true;
}