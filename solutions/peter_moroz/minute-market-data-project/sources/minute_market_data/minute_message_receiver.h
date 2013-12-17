#ifndef MULTICAST_COMMUNICATION_MINUTE_MESSAGE_RECEIVER_H_
#define MULTICAST_COMMUNICATION_MINUTE_MESSAGE_RECEIVER_H_

#include "minute_datafeed.h"
#include <boost/shared_ptr.hpp>

namespace multicast_communication
{

// abstract class - interface for all handlers of minute data messages
class minute_message_receiver
{
public:
  virtual void minute_message(const minute_message_data& m) = 0;
};

typedef boost::shared_ptr<minute_message_receiver> minute_message_receiver_ptr;

}

#endif // MULTICAST_COMMUNICATION_MINUTE_MESSAGE_RECEIVER_H_
 