#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "Date.h"

class Message {


public:
  Message();
  ~Message();

  void CalculateDays();

  void ReadFrom(std::istream& is);
  void WriteTo(std::ostream& os) const;

private:
  char stock_name_[9];
  char date_time_[8];
  double price_;
  double vwap_;
  unsigned volume_;
  double f1_;
  double t1_;
  double f2_;
  double f3_;
  double f4_;

  unsigned days_since_christmas_;

private:
  Message(const Message&);
  const Message& operator=(const Message&);
};

inline void Message::CalculateDays() {
  Date d = Date::ParseString(date_time_);
  days_since_christmas_ = d.GetDaysSinceChristmas();
}

#endif // MESSAGE_H_
