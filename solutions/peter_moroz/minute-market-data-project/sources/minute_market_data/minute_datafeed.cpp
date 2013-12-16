#include "minute_datafeed.h"


multicast_communication::minute_datafeed::minute_datafeed()
  : minute_(0), open_price_(0.0), high_price_(0.0), low_price_(0.0),
    close_price_(0.0), volume_(0.0), bid_(0.0), ask_(0.0)    
{
  ::memset(stock_name_, 0, sizeof(stock_name_));
}
multicast_communication::minute_datafeed::~minute_datafeed()
{
}

void multicast_communication::minute_datafeed::copy_from(const minute_datafeed& md)
{
  minute_ = md.minute_;
  ::strncpy(stock_name_, md.stock_name_, kStockNameLength);
  open_price_ = md.open_price_;
  high_price_ = md.high_price_;
  low_price_ = md.low_price_;
  close_price_ = md.close_price_;
  volume_ = md.volume_;
  bid_ = md.bid_;
  ask_ = md.ask_;
}

std::string multicast_communication::minute_datafeed::get_stock_name() const
{
  return std::string(stock_name_);
}

void multicast_communication::minute_datafeed::read_from(std::ifstream& ifs)
{
  ifs.read(reinterpret_cast<char*>(&minute_), sizeof(minute_));
  ifs.read(stock_name_, sizeof(stock_name_));
  ifs.read(reinterpret_cast<char*>(&open_price_), sizeof(open_price_));
  ifs.read(reinterpret_cast<char*>(&high_price_), sizeof(high_price_));
  ifs.read(reinterpret_cast<char*>(&low_price_), sizeof(low_price_));
  ifs.read(reinterpret_cast<char*>(&close_price_), sizeof(close_price_));
  ifs.read(reinterpret_cast<char*>(&volume_), sizeof(volume_));
  ifs.read(reinterpret_cast<char*>(&bid_), sizeof(bid_));
  ifs.read(reinterpret_cast<char*>(&ask_), sizeof(ask_));
}
void multicast_communication::minute_datafeed::write_to(std::ofstream& ofs) const
{
  ofs.write(reinterpret_cast<const char*>(&minute_), sizeof(minute_));
  ofs.write(stock_name_, sizeof(stock_name_));
  ofs.write(reinterpret_cast<const char*>(&open_price_), sizeof(open_price_));
  ofs.write(reinterpret_cast<const char*>(&high_price_), sizeof(high_price_));
  ofs.write(reinterpret_cast<const char*>(&low_price_), sizeof(low_price_));
  ofs.write(reinterpret_cast<const char*>(&close_price_), sizeof(close_price_));
  ofs.write(reinterpret_cast<const char*>(&volume_), sizeof(volume_));
  ofs.write(reinterpret_cast<const char*>(&bid_), sizeof(bid_));
  ofs.write(reinterpret_cast<const char*>(&ask_), sizeof(ask_));
}
