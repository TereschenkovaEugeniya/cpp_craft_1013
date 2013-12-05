#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "Date.h"
#include <string>
#include <boost/cstdint.hpp>

class Message {

public:
  Message();
  ~Message();

  Message(const Message&);
  const Message& operator=(const Message&);

  void CalculateDays() const;

  void ReadFrom(std::istream& is);
  void WriteTo(std::ostream& os) const;

  std::string stock_name() const {
    return std::string(stock_name_);
  }
                                                        
private:
  char stock_name_[9];
  char date_time_[8];
  double price_;
  double vwap_;
  boost::uint32_t volume_;
  double f1_;
  double t1_;
  double f2_;
  double f3_;
  double f4_;

  mutable boost::uint32_t days_since_christmas_;
};

inline void Message::CalculateDays() const {
  Date d = Date::ParseString(date_time_);
  days_since_christmas_ = d.GetDaysSinceChristmas();
}

#endif // MESSAGE_H_
