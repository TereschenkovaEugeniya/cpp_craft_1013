#ifndef MULTICAST_COMMUNICATION_MARKET_MESSAGE_ROUTER_H_
#define MULTICAST_COMMUNICATION_MARKET_MESSAGE_ROUTER_H_

#include "market_message_receiver.h"

namespace multicast_communication
{

// abstract class - interface for
// all kind of messages routers
class market_message_router
{
protected:
  market_message_router(const market_message_receiver_ptr& dst) : dst_(dst) {}

public:
  virtual void route_message(const std::string& m) = 0;

protected:
  market_message_receiver_ptr dst_;
};

typedef boost::shared_ptr<market_message_router> market_message_router_ptr;



class quote_messages_router : public market_message_router
{
public:
  explicit quote_messages_router(const market_message_receiver_ptr& dst)
    : market_message_router(dst) {}

public:
  virtual void route_message(const std::string& m)
  {
    dst_->quote_message(m);
  }
};


class trade_messages_router : public market_message_router
{
public:
  explicit trade_messages_router(const market_message_receiver_ptr& dst)
    : market_message_router(dst) {}

public:
  virtual void route_message(const std::string& m)
  {
    dst_->trade_message(m);
  }
};

}

#endif // MULTICAST_COMMUNICATION_MARKET_MESSAGE_ROUTER_H_

