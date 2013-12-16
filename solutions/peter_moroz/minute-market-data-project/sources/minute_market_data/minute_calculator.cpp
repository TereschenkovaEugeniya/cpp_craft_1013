#include <algorithm>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "minute_calculator.h"
#include "minute_datafeed.h"

namespace multicast_communication
{
 
struct fire_minute_message 
  : public std::binary_function<minute_message_receiver_ptr, minute_message_data, void>
{
  void operator()(minute_message_receiver_ptr receiver, 
                  const minute_message_data& data) const
  {
    receiver->minute_message(data);
  }
};


// helper, used for creation minute_message in some stages
// I've used Builder pattern here
class minute_message_composer
{
public:
  minute_message_composer(
      const std::vector<quote_message_ptr>& qmessages,
      const std::vector<trade_message_ptr>& tmessages)
   : qmessages_(qmessages), tmessages_(tmessages) {}
  ~minute_message_composer() {}

public:
  void create_minute_message(minute_message_data& data);

private:
  void separate_data_by_markets();
  minute_datafeed get_market_data(const std::string& market);
  

private:
  std::set<std::string> markets_;
  const std::vector<quote_message_ptr> qmessages_;
  const std::vector<trade_message_ptr> tmessages_;
  std::map<std::string, std::vector<quote_message_ptr> > quotas_;
  std::map<std::string, std::vector<trade_message_ptr> > trades_;
};

}


void multicast_communication::minute_calculator::quote_message(const quote_message_ptr& message)
{
  using namespace boost::posix_time;
  ptime message_time = message->get_time();
  if (open_time_ == boost::posix_time::ptime())
    open_time_ = message_time;
  else
  {
    time_period delta(open_time_, message_time);
    if (minutes(1) < delta.length() || minutes(1) == delta.length())
    {
      notify_listeners();
      reset_statistics();
      open_time_ = message_time;  
    }
  }

  qmessages_.push_back(message);
}
void multicast_communication::minute_calculator::trade_message(const trade_message_ptr& message)
{
  using namespace boost::posix_time;
  ptime message_time = message->get_time();
  if (open_time_ == boost::posix_time::ptime())
    open_time_ = message_time;
  else
  {
    time_period delta(open_time_, message_time);
    if (minutes(1) < delta.length() || minutes(1) == delta.length())
    {
      notify_listeners();
      reset_statistics();
      open_time_ = message_time;  
    }
  }

  tmessages_.push_back(message);
}

void multicast_communication::minute_calculator::add_listener(minute_message_receiver_ptr& mmr)
{
  if (std::find(listeners_.begin(), listeners_.end(), mmr) == listeners_.end())
    listeners_.push_back(mmr);
}

void multicast_communication::minute_calculator::stop()
{
  notify_listeners();
}

void multicast_communication::minute_calculator::reset_statistics()
{
  qmessages_.clear();
  tmessages_.clear();
}

void multicast_communication::minute_calculator::notify_listeners() const
{
  minute_message_composer message_composer(qmessages_, tmessages_);

  minute_message_data mm_data;
  message_composer.create_minute_message(mm_data);

  std::for_each(listeners_.begin(), listeners_.end(), 
      std::bind2nd(fire_minute_message(), mm_data));
}


void multicast_communication::minute_message_composer::create_minute_message(
                        multicast_communication::minute_message_data& message)
{
  separate_data_by_markets();
  std::set<std::string>::iterator it = markets_.begin();
  while (it != markets_.end())
  {
    try {
      message.push_back(get_market_data(*it));
    } catch (std::logic_error&) {
      ; // just skip market
    }
    ++it;
  }
}

void multicast_communication::minute_message_composer::separate_data_by_markets()
{
  markets_.clear();
  quotas_.clear();
  trades_.clear();
  for (size_t i = 0; i < qmessages_.size(); ++i)
  {
    const std::string market_name = qmessages_[i]->security_symbol();
    markets_.insert(market_name);
    quotas_[market_name].push_back(qmessages_[i]);
  }

  for (size_t i = 0; i < tmessages_.size(); ++i)
  {
    const std::string market_name = tmessages_[i]->security_symbol();
    markets_.insert(market_name);
    trades_[market_name].push_back(tmessages_[i]);
  }
}
multicast_communication::minute_datafeed 
    multicast_communication::minute_message_composer::get_market_data(const std::string& market)
{
  std::vector<trade_message_ptr> market_trades = trades_[market];
  std::vector<quote_message_ptr> market_quotas = quotas_[market];

  minute_datafeed data;
  data.set_stock_name(market.c_str());

  // it may happens, that no one trade-message has been received
  if (market_trades.size() > 0)
  {
    boost::uint32_t ts_open = market_trades[0]->get_timestamp();
    boost::uint32_t ts_close = market_trades[0]->get_timestamp();
    size_t open_msg_pos = 0;
    size_t close_msg_pos = 0;

    double high_price = 0.0;
    double low_price = 100000000.0;
    double volume = 0.0;
    for (size_t i = 0; i < market_trades.size(); ++i)
    {
      if (market_trades[i]->price() >= high_price)
        high_price = market_trades[i]->price();
      else if (market_trades[i]->price() < low_price)
        low_price = market_trades[i]->price();
      
      if (market_trades[i]->get_timestamp() < ts_open)
      {
        ts_open = market_trades[i]->get_timestamp();
        open_msg_pos = i;      
      }
      else if (market_trades[i]->get_timestamp() > ts_close)
      {
        ts_close = market_trades[i]->get_timestamp();
        close_msg_pos = i;
      }
      
      volume += market_trades[i]->volume();
    }

    using namespace boost::posix_time;
    using namespace boost::gregorian;
    ptime begin_of_20_century(date(1900, Jan, 01));
    time_period tp(begin_of_20_century, market_trades[open_msg_pos]->get_time());
    boost::uint32_t seconds_count = static_cast<boost::uint32_t>(tp.length().total_seconds());

    data.set_minute(seconds_count);
    data.set_open_price(market_trades[open_msg_pos]->price());
    data.set_high_price(high_price);
    data.set_low_price(low_price);
    data.set_close_price(market_trades[close_msg_pos]->price());
    data.set_volume(volume);
  }
 
  double ask = 0.0, bid = 0.0;
  for (size_t i = 0; i < market_quotas.size(); ++i)
  {
    ask += market_quotas[i]->offer_price();
    bid += market_quotas[i]->bid_price();
  }

  data.set_ask(ask);
  data.set_bid(bid);
  
  return data;
}

