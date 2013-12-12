#include <iostream>
#include "minute_market_data.h"

bool multicast_communication::minute_market_data::start()
{
  quote_messages_processor_.reset(new quote_messages_processor());
  trade_messages_processor_.reset(new trade_messages_processor());

  minute_calculator_.reset(new minute_calculator());
  multicast_communication_.reset(new multicast_communication());

  if (!multicast_communication_->initialize())
  {
    std::cerr << "Can't initialize instance of multicast_communication\n";
    return false;
  }

  minute_message_receiver_ptr self_1 = shared_from_this();
  minute_calculator_->add_listener(self_1);
  market_message_receiver_ptr self_2 = shared_from_this();
  multicast_communication_->add_listener(self_2);

  multicast_communication_->start();

  return true;
}
void multicast_communication::minute_market_data::stop()
{  
  {
    boost::mutex::scoped_lock lock(minute_calculator_guard_);
    minute_calculator_->stop();
  }
  if (stop_handler_)
    stop_handler_();
}

void multicast_communication::minute_market_data::quote_message(const std::string& m)
{
  quote_message_ptr qmessage = quote_messages_processor_->parse_message(m);
  if (qmessage)
  {
      boost::mutex::scoped_lock lock(minute_calculator_guard_);
      minute_calculator_->quote_message(qmessage);
  }
}
void multicast_communication::minute_market_data::trade_message(const std::string& m)
{
  trade_message_ptr tmessage = trade_messages_processor_->parse_message(m);
  if (tmessage)
  {
      boost::mutex::scoped_lock lock(minute_calculator_guard_);
      minute_calculator_->trade_message(tmessage);
  }
}
void multicast_communication::minute_market_data::minute_message(
                                     const minute_message_data& m)
{
  if (minute_message_handler_)
    minute_message_handler_(m);
}
