#ifndef MULTICAST_COMMUNICTAION_QUOTE_MESSAGE_H_
#define MULTICAST_COMMUNICTAION_QUOTE_MESSAGE_H_

#include <boost/shared_ptr.hpp>
#include "message.h"


namespace multicast_communication
{

class quote_message : public message
{
public:
  explicit quote_message(const std::string timestamp, const std::string& security_symbol, 
                         const double& bid_price, const double& bid_volume,
                         const double& offer_price, const double& offer_volume);

  const double& bid_price() const { return bid_price_; }
  const double& bid_volume() const { return bid_volume_; }
  const double& offer_price() const { return offer_price_; }
  const double& offer_volume() const { return offer_volume_; }

private:
  double bid_price_;
  double bid_volume_;
  double offer_price_;
  double offer_volume_;
};

typedef boost::shared_ptr<quote_message> quote_message_ptr;

}

#endif MULTICAST_COMMUNICTAION_QUOTE_MESSAGE_H_