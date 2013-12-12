#include <stdexcept>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include "message.h"


boost::uint32_t multicast_communication::message::get_timestamp() const
{
  boost::uint32_t h = 0, m = 0, s = 0, ms = 0;
  parse_timestamp(h, m, s, ms);
  return ((((h * 60 + m) * 60) + s) * 1000 + ms);
}
boost::posix_time::ptime multicast_communication::message::get_time() const
{
  boost::uint32_t h = 0, m = 0, s = 0, ms = 0;
  parse_timestamp(h, m, s, ms);
  using namespace boost::posix_time;
  using namespace boost::gregorian;
  return ptime(day_clock::local_day(), hours(h) 
    + minutes(m) + seconds(s) + milliseconds(ms));
}

void multicast_communication::message::parse_timestamp(boost::uint32_t& h,
        boost::uint32_t& m, boost::uint32_t& s, boost::uint32_t& ms) const
{
  // Time Stamp conversion table is just part of ASCII table, so nothing special
  h = timestamp_[0] - '0';
  m = timestamp_[1] - '0';
  s = timestamp_[2] - '0';
  try {
    ms = boost::lexical_cast<boost::uint32_t>(timestamp_.c_str() + 3);
  } catch (boost::bad_lexical_cast& ex) {
    std::stringstream msg;
    msg << "multicast_communication::message::parse_timestamp failed. "
        << "Reason: " << ex.what();
    throw std::logic_error(msg.str());
  }
}
