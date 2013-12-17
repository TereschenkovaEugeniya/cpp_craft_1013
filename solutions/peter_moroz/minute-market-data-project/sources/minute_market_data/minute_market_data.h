#ifndef MULTICAST_COMMUNICATION_MINUTE_MARKET_DATA_H_
#define MULTICAST_COMMUNICATION_MINUTE_MARKET_DATA_H_

#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "market_message_receiver.h"
#include "minute_message_receiver.h"
#include "multicast_communication.h"
#include "minute_calculator.h"
#include "minute_datafeed.h"
#include "quote_messages_processor.h"
#include "trade_messages_processor.h"


namespace multicast_communication
{

class minute_market_data : public boost::noncopyable,		// disallow copy and assign
                           public market_message_receiver,  // implement interface
                           public minute_message_receiver,	// implement interface
                           public boost::enable_shared_from_this<minute_market_data>
{
public:
  minute_market_data() {}
  ~minute_market_data() {}

  bool start();
  void stop();

  // implementation of market_message_receiver-interface
  virtual void quote_message(const std::string& m);
  virtual void trade_message(const std::string& m);
  // implementation of minute_message_receiver-interface
  virtual void minute_message(const minute_message_data& m);

  /*void set_stop_handler(boost::function<void ()>& stop_handler)
  {
    stop_handler_ = stop_handler;
  }
  void set_minute_message_handler(
      boost::function<void (const minute_message_data& m)>& minute_message_handler)
  {
    minute_message_handler_ = minute_message_handler;
  }*/

  void set_stop_handler(boost::function<void ()> stop_handler)
  {
    stop_handler_ = stop_handler;
  }
  void set_minute_message_handler(
      boost::function<void (const minute_message_data& m)> minute_message_handler)
  {
    minute_message_handler_ = minute_message_handler;
  }

private:
  boost::function<void ()> stop_handler_;
  boost::function<void (const minute_message_data& m)> minute_message_handler_;
  boost::mutex minute_calculator_guard_;
  minute_calculator_ptr minute_calculator_;
  multicast_communication_ptr multicast_communication_;
  boost::scoped_ptr<quote_messages_processor> quote_messages_processor_;
  boost::scoped_ptr<trade_messages_processor> trade_messages_processor_;
};

typedef boost::shared_ptr<minute_market_data> minute_market_data_ptr;

}

#endif // MULTICAST_COMMUNICATION_MINUTE_MARKET_DATA_H_