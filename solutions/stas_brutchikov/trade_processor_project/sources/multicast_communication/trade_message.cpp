#include "trade_message.h"
#include <iomanip>
#include <string>

namespace multicast_communication {

std::string trade_message::security_symbol() const
{
	return security_symbol_;
}

double trade_message::price() const
{
	return price_;
}

double trade_message::volume() const
{
	return volume_;
}

std::ostream& operator << ( std::ostream& out, const trade_message_ptr& msg )
{
      return out << std::fixed << "T " << msg->security_symbol() 
                               << " " << std::setprecision( 2 ) << msg->price() 
                               << " " << std::setprecision( 1 ) << msg->volume() 
                               << std::endl;
}

template<>
bool parse<multicast_communication::trades_messages_ptr>(const std::string& data, trades_messages_ptr& obj)
{
    return true;
}

} //multicast_communication
