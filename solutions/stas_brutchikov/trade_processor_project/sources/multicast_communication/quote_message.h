#ifndef _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_
#define _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_


#include <vector>
#include <boost/shared_ptr.hpp>

#include "base.h"
namespace multicast_communication
{


	class quote_message
	{
    public:
        quote_message();
        quote_message(std::istream& in, message_type type);

		std::string security_symbol() const;
		double bid_price() const;
		double bid_volume() const;
		double offer_price() const;
		double offer_volume() const;

        message_type type() const {return type_;};
    private:
        std::string security_symbol_;      
        double bid_price_;
        double bid_volume_;        
        double offer_price_; 
        double offer_volume_; 
        const message_type type_;
	};

	typedef boost::shared_ptr< quote_message > quote_message_ptr;

   // quote_message_ptr build_quote( std::istream& in );

    typedef std::vector< quote_message_ptr > quote_messages_ptr;

    std::ostream& operator << ( std::ostream& out, const quote_message_ptr& msg );

}

#endif // _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_
