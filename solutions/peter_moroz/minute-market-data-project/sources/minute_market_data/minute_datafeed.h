#ifndef MULTICAST_COMMUNICATION_MINUTE_DATAFEED_H_
#define MULTICAST_COMMUNICATION_MINUTE_DATAFEED_H_

#include <fstream>
#include <string>
#include <vector>

#include <boost/cstdint.hpp>


namespace multicast_communication
{

class minute_datafeed
{
  static const size_t kStockNameLength = 16;
public:
  minute_datafeed();
  ~minute_datafeed();

  minute_datafeed(const minute_datafeed& md)
  {
    copy_from(md);
  }
  const minute_datafeed& operator=(const minute_datafeed& md)
  {
    if (this != &md)
      copy_from(md);
    return *this;
  }

private:
  void copy_from(const minute_datafeed& md);

public:
  boost::uint32_t minute() const { return minute_; }
  const double& open_price() const { return open_price_; }
  const double& high_price() const { return high_price_; }
  const double& low_price() const { return low_price_; }
  const double& close_price() const { return close_price_; }
  const double& volume() const { return volume_; }
  const double& bid() const { return bid_; }
  const double& ask() const { return ask_; }

  std::string get_stock_name() const;
  
  void set_minute(boost::uint32_t minute) { minute_ = minute; }
  void set_stock_name(const char* stock_name) {
    size_t n = (::strlen(stock_name), kStockNameLength - 1);
    ::strncpy(stock_name_, stock_name, n);
  }
  void set_open_price(const double& open_price) { open_price_ = open_price; }
  void set_high_price(const double& high_price) { high_price_ = high_price; }
  void set_low_price(const double& low_price) { low_price_ = low_price; }
  void set_close_price(const double& close_price) { close_price_ = close_price; }
  void set_volume(const double& volume) { volume_ = volume; }
  void set_bid(const double& bid) { bid_ = bid; }
  void set_ask(const double& ask) { ask_ = ask; }
  
  void read_from(std::ifstream& ofs);
  void write_to(std::ofstream& ofs) const;

private:
  boost::uint32_t minute_;
  char stock_name_[kStockNameLength];
  double open_price_;
  double high_price_;
  double low_price_;
  double close_price_;
  double volume_;
  double bid_;
  double ask_;
};

typedef std::vector<minute_datafeed> minute_message_data;

}

#endif // MULTICAST_COMMUNICATION_MINUTE_DATAFEED_H_
                                           