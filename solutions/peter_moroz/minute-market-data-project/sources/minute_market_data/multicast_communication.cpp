#include <algorithm>
#include <functional>

#include <boost/lexical_cast.hpp>

#include "multicast_communication.h"

namespace mcast_comm = multicast_communication;

namespace multicast_communication
{

struct fire_quote_message 
  : public std::binary_function<market_message_receiver_ptr, std::string, void>
{
  void operator()(market_message_receiver_ptr& receiver, const std::string& m) const
  {
    receiver->quote_message(m);
  }
};

struct fire_trade_message 
  : public std::binary_function<market_message_receiver_ptr, std::string, void>
{
  void operator()(market_message_receiver_ptr& receiver, const std::string& m) const
  {
    receiver->trade_message(m);
  }
};

struct stop_message_pump : public std::unary_function<market_message_pump_ptr, void>
{
  void operator()(market_message_pump_ptr& pump)
  {
    pump->stop();
  }
};
  
}

bool mcast_comm::multicast_communication::initialize()
{
  if (!read_configuration())
    return false;

  multicast_communication_ptr self = shared_from_this();
  qmessages_router_.reset(new quote_messages_router(self));
  tmessages_router_.reset(new trade_messages_router(self));

  for (size_t i = 0; i < quote_connections_.size(); ++i)
  {
    market_message_pump_ptr qmessage_pump(
        new market_message_pump(quote_io_service_, qmessages_router_));
    quote_pumps_.push_back(qmessage_pump);
  }

  for (size_t i = 0; i < trade_connections_.size(); ++i)
  {
    market_message_pump_ptr tmessage_pump(
        new market_message_pump(trade_io_service_, tmessages_router_));
    trade_pumps_.push_back(tmessage_pump);
  }

  return true;  
}

void mcast_comm::multicast_communication::add_listener(market_message_receiver_ptr& mmr)
{
  if (std::find(listeners_.begin(), listeners_.end(), mmr) == listeners_.end())
    listeners_.push_back(mmr);
}

void mcast_comm::multicast_communication::start()
{
  for (size_t i = 0; i < quote_connections_.size(); ++i)
  {
    const connection_point cp = quote_connections_[i];
    quote_pumps_[i]->start(cp.first, cp.second);
  }

  for (size_t i = 0; i < trade_connections_.size(); ++i)
  {
    const connection_point cp = trade_connections_[i];
    trade_pumps_[i]->start(cp.first, cp.second);
  }

  for (size_t i = 0; i < quote_thread_count_; ++i)
    workers_.create_thread(boost::bind(&multicast_communication::io_engine, 
                                      this, boost::ref(quote_io_service_)));

  for (size_t i = 0; i < trade_thread_count_; ++i)
    workers_.create_thread(boost::bind(&multicast_communication::io_engine, 
                            this, boost::ref(trade_io_service_)));
}
void mcast_comm::multicast_communication::stop()
{
  std::for_each(quote_pumps_.begin(), quote_pumps_.end(), stop_message_pump());
  std::for_each(trade_pumps_.begin(), trade_pumps_.end(), stop_message_pump());

  quote_io_service_.stop();
  trade_io_service_.stop();
  workers_.join_all();
}

void mcast_comm::multicast_communication::quote_message(const std::string& m)
{
  std::for_each(listeners_.begin(), listeners_.end(), 
		std::bind2nd(fire_quote_message(), m));
}
void mcast_comm::multicast_communication::trade_message(const std::string& m)
{
  std::for_each(listeners_.begin(), listeners_.end(), 
		std::bind2nd(fire_trade_message(), m));
}

bool mcast_comm::multicast_communication::read_configuration()
{
  static const char* kConfigFilename = BINARY_DIR "/config.ini";

  std::ifstream config_ifs(kConfigFilename);
  if (!config_ifs.is_open())
  {
    std::cerr << "Can't open configuration file: " 
	      << kConfigFilename << std::endl; 
    return false;
  }

  std::string line;

  try {
    getline(config_ifs, line);
    trade_thread_count_ = boost::lexical_cast<size_t>(line.c_str());
    getline(config_ifs, line);
    quote_thread_count_ = boost::lexical_cast<size_t>(line.c_str());
  } catch (const boost::bad_lexical_cast& e) {
    std::cerr << "Error in multicast_communication::multicast_communication::read_configuration "
              << "Reason " << e.what() << std::endl;
    return false;
  }

  if (!read_connections(config_ifs, trade_connections_))
    return false;

  return read_connections(config_ifs, quote_connections_);
}
bool mcast_comm::multicast_communication::read_connections(std::ifstream& cfg_ifs,
                                         std::vector<connection_point>& connections)
{
  std::string line;
  size_t connections_count = 0;

  try {
    getline(cfg_ifs, line);
    connections_count = boost::lexical_cast<size_t>(line.c_str());
  } catch (const boost::bad_lexical_cast& e) {
    std::cerr << "Error in multicast_communication::multicast_communication::read_connections "
              << "Reason " << e.what() << std::endl;
    return false;
  }

  connections.clear();
  for (size_t i = 0; i < connections_count; ++i)
  {
    getline(cfg_ifs, line);
    std::string address, port;
    size_t pos = line.find(' ');
    if (pos == std::string::npos)
      pos = line.find('\t');
    if (pos != std::string::npos)
    {
      address = line.substr(0, pos);
      while (pos < line.size() && (line[pos] == ' '
        || line[pos] == '\t'))
      { 
        ++pos;
      }

      unsigned short p = 0;
      if (pos < line.size())
        port = line.substr(pos);
      try {
        p = boost::lexical_cast<unsigned short>(port.c_str());
      } catch (boost::bad_lexical_cast& e) {
        std::cerr << "Error in multicast_communication::multicast_communication::read_connections "
                  << "Reason " << e.what() << std::endl;
        return false;
      }

      connections.push_back(connection_point(address, p));      
    } // if (pos != std::string::npos)
  } // for (size_t i = 0; i < connections_count; ++i)

  return true;
}

void mcast_comm::multicast_communication::io_engine(boost::asio::io_service& service)
{
  service.run();
}
