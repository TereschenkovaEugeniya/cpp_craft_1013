#include "trade_message.h"

multicast_communication::trade_message::trade_message() : security_symbol_(""), price_(0), volume_(0)
{
	denominators_ = boost::assign::map_list_of( '3' , 8.0 ) ( '4' , 16.0 ) ( '5' , 32.0 ) ( '6' , 64.0 ) ( '7' , 128.0 ) ( '8' , 256.0 )
		( 'A' , 10.0 ) ( 'B' , pow( 10.0, 2 ) ) ( 'C' , pow( 10.0, 3 ) ) ( 'D' , pow( 10.0, 4 ) ) ( 'E' , pow( 10.0, 5 ) ) ( 'F' , pow( 10.0, 6 ) ) ( 'G' , pow( 10.0, 7 ) ) ( 'H' , pow( 10.0, 8 ) );

	uint32_t time = 0;
	for (char ch = 48 ; ch < 108; ++ch, ++time)
	{
		time_stamp_[ch] = time;
	}
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

uint32_t multicast_communication::trade_message::time() const
{
	return time_;
}

bool multicast_communication::trade_message::initialize(const std::string& msg)
{
	if (msg.length() < 44)
	{
		return false;
	}

    try
    {
	    std::string header = msg.substr(0, 24);
	    std::string body = msg.substr(24, msg.length() - 24);

	    std::string time = header.substr(18, 3);
	    time_ = 3600 * time_stamp_[time[0]] + 60 * time_stamp_[time[1]] + time_stamp_[time[2]];

	    if ((header[0] == 'E' || header[0] == 'L') && header[1] == 'I')
	    {
		    return init_short(body);
	    }
	    else if ((header[0] == 'B' || header[0] == 'E' || header[0] == 'L') && header[1] == 'B')
	    {
		    return init_long(body);
	    }
    }
    catch(...)
    {
        return false;
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
	double denominator = 1;
	try
	{
		denominator = denominators_.at(body[8]);
	}
	catch(std::out_of_range)
	{
        denominator = 1;
	}
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
		
	double denominator = 1;
	try
	{
		denominator = denominators_.at(body[32]);
	}
	catch(std::out_of_range)
	{
        denominator = 1;
	}
	std::string price = body.substr(33, 12);
	std::string trade_volume = body.substr(45, 9);

	price_ = boost::lexical_cast<double>(price)/denominator;
	volume_ = boost::lexical_cast<double>(trade_volume);

    return true;
}