#ifndef _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_
#define _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_


#include <list>
#include <boost/shared_ptr.hpp>

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


    typedef std::list< trade_message_ptr > trades_messages_ptr;

    std::ostream& operator << ( std::ostream& out, const trade_message_ptr& msg );

    template<>
    bool parse<trades_messages_ptr>(const string& data, trades_messages_ptr& obj)
    {
        return true;
    }
}

#endif // _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_
