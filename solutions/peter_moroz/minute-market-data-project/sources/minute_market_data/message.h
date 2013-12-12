#ifndef MULTICAST_COMMUNICATION_MESSAGE_H_
#define MULTICAST_COMMUNICATION_MESSAGE_H_

#include <string>
#include <boost/date_time.hpp>

namespace multicast_communication
{

class message
{
protected:
  message(const std::string& timestamp, const std::string& security_symbol)
    : timestamp_(timestamp), security_symbol_(security_symbol) {}

public:
  virtual ~message() {}

public:
  boost::uint32_t get_timestamp() const;
  boost::posix_time::ptime get_time() const;

  const std::string& security_symbol() const 
  {
    return security_symbol_;  
  }

private:
  void parse_timestamp(boost::uint32_t& h, boost::uint32_t& m,
                       boost::uint32_t& s, boost::uint32_t& ms) const;

private:
  std::string timestamp_;
  std::string security_symbol_;
};

}


#endif // MULTICAST_COMMUNICATION_MESSAGE_H_