#ifndef _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_
#define _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_

#include <list>
#include <boost/shared_ptr.hpp>

namespace multicast_communication
{


	class quote_message
	{
	public:
		std::string security_symbol() const;
		double bid_price() const;
		double bid_volume() const;
		double offer_price() const;
		double offer_volume() const;
	};

	typedef boost::shared_ptr< quote_message > quote_message_ptr;


    typedef std::list< quote_message_ptr > quote_messages_ptr;

    std::ostream& operator << ( std::ostream& out, const quote_message_ptr& msg );

    template<>
    bool parse<quote_messages_ptr>(const string& data, quote_messages_ptr& obj)
    {
        return true;
    }
}

#endif // _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_
