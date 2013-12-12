#include "quote_message.h"

multicast_communication::quote_message::quote_message(
    const std::string timestamp, const std::string& security_symbol, 
    const double& bid_price, const double& bid_volume, 
    const double& offer_price, const double& offer_volume) 
  : message(timestamp, security_symbol),
    bid_price_(bid_price), bid_volume_(bid_volume),
    offer_price_(offer_price), offer_volume_(offer_volume) {}

