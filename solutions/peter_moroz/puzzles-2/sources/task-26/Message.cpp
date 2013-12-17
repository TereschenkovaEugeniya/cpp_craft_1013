#include "Message.h"
#include <iostream>

using namespace std;


Message::Message() {
  ::memset(stock_name_, 0, sizeof(stock_name_));
  ::memset(date_time_, 0, sizeof(stock_name_));
  price_ = 0.0;
  vwap_ = 0.0;
  volume_ = 0;
  f1_ = 0.0;
  t1_ = 0.0;
  f2_ = 0.0;
  f3_ = 0.0;
  f4_ = 0.0;
  days_since_christmas_ = 0;
}
Message::~Message() {}

void Message::ReadFrom(istream& is) {
  is.read(stock_name_, sizeof(stock_name_) - 1);
  is.read(date_time_, sizeof(date_time_));
  is.read(reinterpret_cast<char*>(&price_), sizeof(price_));
  is.read(reinterpret_cast<char*>(&vwap_), sizeof(vwap_));
  is.read(reinterpret_cast<char*>(&volume_), sizeof(volume_));
  
  is.read(reinterpret_cast<char*>(&f1_), sizeof(f1_));
  is.read(reinterpret_cast<char*>(&t1_), sizeof(t1_));
  is.read(reinterpret_cast<char*>(&f2_), sizeof(f2_));
  is.read(reinterpret_cast<char*>(&f3_), sizeof(f3_));
  is.read(reinterpret_cast<char*>(&f4_), sizeof(f4_));
}
void Message::WriteTo(ostream& os) const {
  os.write(stock_name_, sizeof(stock_name_));
  os.write(reinterpret_cast<const char*>(&days_since_christmas_), sizeof(days_since_christmas_));
  os.write(reinterpret_cast<const char*>(&vwap_), sizeof(vwap_));
  os.write(reinterpret_cast<const char*>(&volume_), sizeof(volume_));
  os.write(reinterpret_cast<const char*>(&f2_), sizeof(f2_));
}
