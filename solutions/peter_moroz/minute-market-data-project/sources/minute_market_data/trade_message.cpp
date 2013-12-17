#include "trade_message.h"

multicast_communication::trade_message::trade_message(
    const std::string& timestamp, const std::string& security_symbol,
    const double& price, const double& volume) 
  : message(timestamp, security_symbol), price_(price), volume_(volume) {}
