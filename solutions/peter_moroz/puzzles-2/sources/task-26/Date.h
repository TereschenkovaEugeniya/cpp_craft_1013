#ifndef DATE_H_
#define DATE_H_

#include <string>
#include <boost/cstdint.hpp>

class Date {

  static const boost::uint32_t kDaysInMonth = 31;
  static const boost::uint32_t kMonthsInYear = 12;
  static const boost::uint32_t kDaysInYear = kDaysInMonth * kMonthsInYear;

private:
  Date() : d_(0), m_(0), y_(0) {}

public:
  explicit Date(const char* s);

  static Date ParseString(const char* s);

private:
  static bool CheckDayValue(boost::uint32_t dv) {
    return (dv > 0 && dv <= kDaysInMonth);
  }
  static bool CheckMonthValue(boost::uint32_t mv) {
    return (mv > 0 && mv <= kMonthsInYear);
  }
  static bool CheckYearValue(boost::uint32_t yv) {
    return (yv > 0);
  }

public:
  boost::uint32_t GetDaysSinceChristmas() const {
    return ((y_ - 1) * kDaysInYear + (m_ - 1) * kDaysInMonth + d_);
  }

private:
  boost::uint32_t d_;
  boost::uint32_t m_;
  boost::uint32_t y_;
};

#endif // DATE_H_
