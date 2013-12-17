#ifndef MULTICAST_COMMUNICATION_MARKET_MESSAGE_RECEIVER_H_
#define MULTICAST_COMMUNICATION_MARKET_MESSAGE_RECEIVER_H_

#include <string>
#include <boost/shared_ptr.hpp>

namespace multicast_communication
{

// abstract class - interface for 
// handlers of received messages
class market_message_receiver
{
public:
  virtual void quote_message(const std::string& m) = 0;
  virtual void trade_message(const std::string& m) = 0;
};

typedef boost::shared_ptr<market_message_receiver> market_message_receiver_ptr;

}

#endif // MULTICAST_COMMUNICATION_MARKET_MESSAGE_RECEIVER_H_
