#include <assert.h>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include "Date.h"

using namespace std;

Date::Date(const char* s) {
  (*this) = ParseString(s);
}
Date Date::ParseString(const char* s) {
  assert(s != NULL);
  stringstream msg;
  Date d;

  if (::strlen(s) < 8) {
    msg << "Date::ParseString - invalid date string: "
      << s << ", it must has format YYYYMMDD" << endl;
    throw logic_error(msg.str());
  }

  ::sscanf(s, "%4u%2u%2u", &d.y_, &d.m_, &d.d_);

  if (!CheckYearValue(d.y_)) {
    msg << "Date::ParseString - invalid year value, "
        << "it must be greater than 0" << endl;
    throw logic_error(msg.str());
  }

  if (!CheckMonthValue(d.m_)) {
    msg << "Date::ParseString - invalid month value, "
        << "it must be in range [1; 12] " << endl;
    throw logic_error(msg.str());
  }

  if (!CheckDayValue(d.d_)) {
    msg << "Date::ParseString - invalid day value, "
      	<< "it must be in range [1; 31] " << endl;
    throw logic_error(msg.str());
  }
  
  return d;
}
