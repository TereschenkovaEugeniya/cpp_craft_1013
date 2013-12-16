#ifndef MULTICAST_COMMUNICATION_MINUTE_CALCULATOR_H_
#define MULTICAST_COMMUNICATION_MINUTE_CALCULATOR_H_

#include <vector>

#include <boost/shared_ptr.hpp>

#include "minute_message_receiver.h"
#include "quote_message.h"
#include "trade_message.h"

namespace multicast_communication
{

class minute_calculator : public boost::noncopyable	// disallow copy and assign
{

public:
  minute_calculator() {}
  ~minute_calculator() {}

  void quote_message(const quote_message_ptr& message);
  void trade_message(const trade_message_ptr& message);

  void add_listener(minute_message_receiver_ptr& mmr);

  void stop();

private:
  void reset_statistics();
  void notify_listeners() const;
  
private:
  boost::posix_time::ptime open_time_;	// time, which has opened minute
  std::vector<minute_message_receiver_ptr> listeners_;
  std::vector<quote_message_ptr> qmessages_;
  std::vector<trade_message_ptr> tmessages_;
};

typedef boost::shared_ptr<minute_calculator> minute_calculator_ptr;

}

#endif // MULTICAST_COMMUNICATION_MINUTE_CALCULATOR_H_
                                           