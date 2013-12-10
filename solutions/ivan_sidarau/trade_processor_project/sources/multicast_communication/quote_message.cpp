#include "quote_message.h"


const std::string& multicast_communication::quote_message::security_symbol() const
{
	return security_symbol_;
}

double multicast_communication::quote_message::bid_price() const
{
	return 0.0;
}

double multicast_communication::quote_message::bid_volume() const
{
	return 0.0;
}

double multicast_communication::quote_message::offer_price() const
{
	return 0.0;
}

double multicast_communication::quote_message::offer_volume() const
{
	return 0.0;
}
