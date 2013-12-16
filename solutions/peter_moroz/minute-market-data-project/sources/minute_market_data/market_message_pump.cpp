#include <boost/bind.hpp>
#include "market_message_pump.h"

namespace mcast_comm = multicast_communication;

mcast_comm::market_message_pump::market_message_pump(boost::asio::io_service& io_service,
  const market_message_router_ptr& message_router)
  : socket_(io_service), message_router_(message_router) {}

mcast_comm::market_message_pump::~market_message_pump() {}

void mcast_comm::market_message_pump::start(const std::string& addr, unsigned short port)
{
  using boost::asio::ip::udp;
  using boost::asio::ip::address;
  using boost::asio::ip::multicast::join_group;
  
  boost::asio::ip::udp::endpoint ep(address::from_string("0.0.0.0"), port);
  socket_.open(ep.protocol());
  socket_.set_option(udp::socket::reuse_address(true));
  socket_.bind(ep);
  socket_.set_option(join_group(address::from_string(addr)));

  receive();
}
void mcast_comm::market_message_pump::stop()
{
  socket_.close();
}

void mcast_comm::market_message_pump::parse_transmission_block(
                              std::vector<std::string>& messages)
{
  const char SOH = 0x01;
  const char ETX = 0x03;
  const char US = 0x1F;

  size_t p0 = transmission_block_.find(SOH);
  size_t p1 = 0;

  //std::string message;

  messages.clear();
  
  while (p0 != std::string::npos)
  {
    ++p0;
    if (p0 >= transmission_block_.length())
      break;
    p1 = transmission_block_.find(US, p0);
    if (p1 == std::string::npos)
    {
      p1 = transmission_block_.find(ETX, p0);
      if (p1 == std::string::npos)
        break;
    }
    messages.push_back(transmission_block_.substr(p0, p1 - p0));
    p0 = p1;
  } // while (p0 != std::string::npos)
}

void mcast_comm::market_message_pump::receive()
{
  using namespace boost::asio::placeholders;

  socket_.async_receive(boost::asio::buffer(recv_buffer_),
            boost::bind(&market_message_pump::on_receive, 
            this, recv_buffer_, error, bytes_transferred));
}
void mcast_comm::market_message_pump::on_receive(
    char* buffer, const sys_err_code& error, const size_t recv_bytes_count)
{
  if (error)
    return;
  transmission_block_.assign(recv_buffer_, recv_bytes_count);
  transfer_data();
  receive();
}

void mcast_comm::market_message_pump::transfer_data()
{
  std::vector<std::string> messages;
  parse_transmission_block(messages);

  for (size_t i = 0; i < messages.size(); ++i)
    message_router_->route_message(messages[i]);
}
