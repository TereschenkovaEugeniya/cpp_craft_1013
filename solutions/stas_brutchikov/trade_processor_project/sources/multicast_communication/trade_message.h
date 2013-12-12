#ifndef _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_
#define _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_


#include <vector>
#include <boost/shared_ptr.hpp>

#include "base.h"

namespace multicast_communication
{
	class trade_message
	{    
	public:
		std::string security_symbol() const;
		double price() const;
		double volume() const;

    private:
        std::string security_symbol_;
    	double price_;
		double volume_;
	};

	typedef boost::shared_ptr< trade_message > trade_message_ptr;


    typedef std::vector< trade_message_ptr > trades_messages_ptr;

    std::ostream& operator << ( std::ostream& out, const trade_message_ptr& msg );

}

#endif // _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_
