#include "quote_message.h"
#include <iomanip>
#include <string>

std::string multicast_communication::quote_message::security_symbol() const
{
	return "";
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


std::ostream& multicast_communication::operator << ( std::ostream& out, const quote_message_ptr& msg )
{
    return out << std::fixed << "Q " << msg->security_symbol() 
                             << " " << std::setprecision( 2 ) << msg->bid_price() 
                             << " " << std::setprecision( 1 ) << msg->bid_volume() 
                             << " " << std::setprecision( 2 ) << msg->offer_price() 
                             << " " << std::setprecision( 1 ) << msg->offer_volume() 
                             << std::endl;    
}

template<>
bool multicast_communication::parse<multicast_communication::quote_messages_ptr>(const std::string& data, quote_messages_ptr& obj)
{
    return true;
}