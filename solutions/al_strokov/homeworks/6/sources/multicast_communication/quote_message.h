#ifndef _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_
#define _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_

#include <boost/shared_ptr.hpp>

class quote_message {
public:
	std::string security_symbol;
	double bid_price;
	double bid_volume;
	double offer_price;
	double offer_volume;
};

typedef boost::shared_ptr<quote_message> quote_message_ptr;

#endif // _MULTICAST_COMMUNICATION_QUOTE_MESSAGE_H_
