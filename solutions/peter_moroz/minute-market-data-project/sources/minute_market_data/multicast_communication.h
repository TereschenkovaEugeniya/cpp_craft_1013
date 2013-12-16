#ifndef MULTICAST_COMMUNICATION_MULTICAST_COMMUNICATION_H_
#define MULTICAST_COMMUNICATION_MULTICAST_COMMUNICATION_H_

#include <fstream>
#include <string>
#include <vector>                                       
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "market_message_pump.h"
#include "market_message_receiver.h"
#include "market_message_router.h"

namespace multicast_communication
{

class multicast_communication : public boost::noncopyable, 	// disallow copy and assign
				public market_message_receiver,	// receive market messages
				public boost::enable_shared_from_this<multicast_communication>
{
  typedef std::pair<std::string, unsigned short> connection_point;

public:
  explicit multicast_communication() {}
  virtual ~multicast_communication() {}

  bool initialize();

  void add_listener(market_message_receiver_ptr& mmr);

  void start();
  void stop();

  // implementation of market_messages_receiver-interface
  virtual void quote_message(const std::string& m);
  virtual void trade_message(const std::string& m);

private:
  bool read_configuration();
  static bool read_connections(std::ifstream& cfg_ifs, std::vector<connection_point>& connections);

  void io_engine(boost::asio::io_service& service);

private:
  size_t quote_thread_count_;
  size_t trade_thread_count_;
  std::vector<connection_point> quote_connections_;
  std::vector<connection_point> trade_connections_;

  std::vector<market_message_pump_ptr> quote_pumps_;
  std::vector<market_message_pump_ptr> trade_pumps_;

  market_message_router_ptr qmessages_router_;
  market_message_router_ptr tmessages_router_;
  std::vector<market_message_receiver_ptr> listeners_;
  boost::asio::io_service quote_io_service_;
  boost::asio::io_service trade_io_service_;  
  boost::thread_group workers_;
};

typedef boost::shared_ptr<multicast_communication> multicast_communication_ptr;

}

#endif // MULTICAST_COMMUNICATION_MULTICAST_COMMUNICATION_H_
