#ifndef MULTICAST_COMMUNICTAION_TRADE_MESSAGE_H_
#define MULTICAST_COMMUNICTAION_TRADE_MESSAGE_H_

#include <boost/shared_ptr.hpp>
#include "message.h"

namespace multicast_communication
{

class trade_message : public message
{
public:
  explicit trade_message(const std::string& timestamp,
                         const std::string& security_symbol,
                         const double& price, const double& volume);

  const double& price() const { return price_; }
  const double& volume() const { return volume_; }

private:
  double price_;
  double volume_;
};

typedef boost::shared_ptr<trade_message> trade_message_ptr;

}


#endif // MULTICAST_COMMUNICTAION_TRADE_MESSAGE_H_