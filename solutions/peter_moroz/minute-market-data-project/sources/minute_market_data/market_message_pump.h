#ifndef MULTICAST_COMMUNICATION_MARKET_MESSAGE_PUMP_H_
#define MULTICAST_COMMUNICATION_MARKET_MESSAGE_PUMP_H_

#include <string>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>

#include "market_message_router.h"

namespace multicast_communication
{


// class responsible for receiving data from network
// and parsing transmission block into messages
class market_message_pump : public boost::noncopyable
{
  static const size_t kRecvBufferSize = 1000;
  typedef boost::system::error_code sys_err_code;

public:
  explicit market_message_pump(boost::asio::io_service& io_service,
                               const market_message_router_ptr& mm_router);
  virtual ~market_message_pump();

public:
  void start(const std::string& addr, unsigned short port);
  void stop();

private:

  // split transmission block into messages
  void parse_transmission_block(std::vector<std::string>& messages);

  void receive();
  void on_receive(char* buffer, const sys_err_code& error, 
                  const size_t recv_bytes_count);

  void transfer_data();

private:
  boost::asio::ip::udp::socket socket_;
  market_message_router_ptr message_router_;
  char recv_buffer_[kRecvBufferSize];
  //boost::array<char, kRecvBufferSize> recv_buffer_; 
  std::string transmission_block_;
};

typedef boost::shared_ptr<market_message_pump> market_message_pump_ptr;

}

#endif // MULTICAST_COMMUNICATION_MARKET_MESSAGE_PUMP_H_
