#include "trade_message.h"
#include <iomanip>
#include <string>

std::string multicast_communication::trade_message::security_symbol() const
{
	return "";
}

double multicast_communication::trade_message::price() const
{
	return 0.0;
}

double multicast_communication::trade_message::volume() const
{
	return 0.0;
}

std::ostream& multicast_communication::operator << ( std::ostream& out, const trade_message_ptr& msg )
{
      return out << std::fixed << "T " << msg->security_symbol() 
                               << " " << std::setprecision( 2 ) << msg->price() 
                               << " " << std::setprecision( 1 ) << msg->volume() 
                               << std::endl;
}
