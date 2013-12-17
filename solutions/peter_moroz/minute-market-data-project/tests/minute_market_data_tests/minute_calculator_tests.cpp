#include "test_registrator.h"
#include <iomanip>
#include <map>
#include <string>
#include <sstream>
#include <minute_calculator.h>

namespace multicast_communication
{
  namespace tests_
  {
    std::string timestamp_as_string(boost::uint8_t h, 
				    boost::uint8_t m,
				    boost::uint8_t s,
				    boost::uint16_t ms);
    quote_message_ptr generate_qmessage(const std::string& secsymbol, 
					boost::uint8_t h, boost::uint8_t m, boost::uint8_t s);
    trade_message_ptr generate_tmessage(const std::string& secsymbol, 
					boost::uint8_t h, boost::uint8_t m, boost::uint8_t s);
    
    void register_message(std::map<std::string, minute_datafeed>& data, quote_message_ptr& m);
    void register_message(std::map<std::string, minute_datafeed>& data, trade_message_ptr& m);
				 	
    class test_minute_message_receiver : public minute_message_receiver
    {
    public:
      test_minute_message_receiver() {}
      ~test_minute_message_receiver() {}

      virtual void minute_message(const minute_message_data& m);
      const std::map<std::string, minute_datafeed>& data() const { return data_; }

    private:
      std::map<std::string, minute_datafeed> data_;
    };
  }
}


void multicast_communication::tests_::minute_calculator_tests()
{
  BOOST_CHECK_NO_THROW
  (
    minute_calculator mc; 
  )

  ::srand(::time(NULL));
  minute_calculator mc;
  boost::shared_ptr<test_minute_message_receiver> test_receiver_p(new test_minute_message_receiver());
  minute_message_receiver_ptr mmr_p = test_receiver_p;
  mc.add_listener(mmr_p);
    
  boost::uint8_t h = ::rand() % 24;
  boost::uint8_t m = ::rand() % 60;

  std::map<std::string, minute_datafeed> data1;
  std::map<std::string, minute_datafeed> data2;
  data1.insert(std::make_pair("AAPL", minute_datafeed()));
  data1.insert(std::make_pair("GOOG", minute_datafeed()));
  data2.insert(std::make_pair("AAPL", minute_datafeed()));
  data2.insert(std::make_pair("GOOG", minute_datafeed()));


  // send quad of messages per second
  for (boost::uint8_t s = 0; s < 60; ++s)
  {
    quote_message_ptr qmessage;
    qmessage = generate_qmessage("AAPL", h, m, s);
    register_message(data1, qmessage);
    mc.quote_message(qmessage);
    qmessage = generate_qmessage("GOOG", h, m, s);
    register_message(data1, qmessage);
    mc.quote_message(qmessage);

    trade_message_ptr tmessage;
    tmessage = generate_tmessage("AAPL", h, m, s);
    register_message(data1, tmessage);
    mc.trade_message(tmessage);
    tmessage = generate_tmessage("GOOG", h, m, s);
    register_message(data1, tmessage);
    mc.trade_message(tmessage);
  }

  
  const std::map<std::string, minute_datafeed>& rd = test_receiver_p->data();
  std::map<std::string, minute_datafeed> received_data;
  BOOST_CHECK_EQUAL(received_data.size(), 0);

  ++m;
  // send some messages in next minute  
  for (boost::uint8_t s = 0; s < 60; ++s)
  {
    quote_message_ptr qmessage;
    qmessage = generate_qmessage("AAPL", h, m, s);
    register_message(data2, qmessage);
    mc.quote_message(qmessage);

    // when we send first message in next minute, all previously
    // received data will be forwarded to instance of listener 
    // so, we have to store received data, 
    // and begin to accumulate them again
    if (s == 0)
      received_data = rd;

    qmessage = generate_qmessage("GOOG", h, m, s);
    register_message(data2, qmessage);
    mc.quote_message(qmessage);

    trade_message_ptr tmessage;
    tmessage = generate_tmessage("AAPL", h, m, s);
    register_message(data2, tmessage);
    mc.trade_message(tmessage);
    tmessage = generate_tmessage("GOOG", h, m, s);
    register_message(data2, tmessage);
    mc.trade_message(tmessage);
  }


  BOOST_CHECK_EQUAL(received_data.size(), 2);

  {
    std::map<std::string, minute_datafeed>::const_iterator it_1 
                                                  = data1.find("AAPL");
    std::map<std::string, minute_datafeed>::const_iterator it_2 
                                                  = received_data.find("AAPL");
    const minute_datafeed& md_orig = it_1->second;
    const minute_datafeed& md_recv = it_2->second;
    BOOST_CHECK_EQUAL(md_recv.get_stock_name(), "AAPL");
    BOOST_CHECK_EQUAL(md_recv.open_price(), md_orig.open_price());
    BOOST_CHECK_EQUAL(md_recv.high_price(), md_orig.high_price());
    BOOST_CHECK_EQUAL(md_recv.low_price(), md_orig.low_price());
    BOOST_CHECK_EQUAL(md_recv.close_price(), md_orig.close_price());
    BOOST_CHECK_EQUAL(md_recv.volume(), md_orig.volume());
    BOOST_CHECK_EQUAL(md_recv.ask(), md_orig.ask());
    BOOST_CHECK_EQUAL(md_recv.bid(), md_orig.bid());
  }

  {
    std::map<std::string, minute_datafeed>::const_iterator it_1 
                                                  = data1.find("GOOG");
    std::map<std::string, minute_datafeed>::const_iterator it_2 
                                                  = received_data.find("GOOG");
    const minute_datafeed& md_orig = it_1->second;
    const minute_datafeed& md_recv = it_2->second;
    BOOST_CHECK_EQUAL(md_recv.get_stock_name(), "GOOG");
    BOOST_CHECK_EQUAL(md_recv.open_price(), md_orig.open_price());
    BOOST_CHECK_EQUAL(md_recv.high_price(), md_orig.high_price());
    BOOST_CHECK_EQUAL(md_recv.low_price(), md_orig.low_price());
    BOOST_CHECK_EQUAL(md_recv.close_price(), md_orig.close_price());
    BOOST_CHECK_EQUAL(md_recv.volume(), md_orig.volume());
    BOOST_CHECK_EQUAL(md_recv.ask(), md_orig.ask());
    BOOST_CHECK_EQUAL(md_recv.bid(), md_orig.bid());
  }
  

  mc.stop();

  {
    std::map<std::string, minute_datafeed>::const_iterator it_2 
                                                  = data2.find("AAPL");
    std::map<std::string, minute_datafeed>::const_iterator it_1 
                                                  = rd.find("AAPL");
    const minute_datafeed& md_orig = it_2->second;
    const minute_datafeed& md_recv = it_1->second;

    BOOST_CHECK_EQUAL(md_recv.get_stock_name(), "AAPL");
    BOOST_CHECK_EQUAL(md_recv.open_price(), md_orig.open_price());
    BOOST_CHECK_EQUAL(md_recv.high_price(), md_orig.high_price());
    BOOST_CHECK_EQUAL(md_recv.low_price(), md_orig.low_price());
    BOOST_CHECK_EQUAL(md_recv.close_price(), md_orig.close_price());
    BOOST_CHECK_EQUAL(md_recv.volume(), md_orig.volume());
    BOOST_CHECK_EQUAL(md_recv.ask(), md_orig.ask());
    BOOST_CHECK_EQUAL(md_recv.bid(), md_orig.bid());
  }

  {
      std::map<std::string, minute_datafeed>::const_iterator it_2 
                                                    = data2.find("GOOG");
      std::map<std::string, minute_datafeed>::const_iterator it_1 
                                                    = rd.find("GOOG");
      const minute_datafeed& md_orig = it_2->second;
      const minute_datafeed& md_recv = it_1->second;

      BOOST_CHECK_EQUAL(md_recv.get_stock_name(), "GOOG");
      BOOST_CHECK_EQUAL(md_recv.open_price(), md_orig.open_price());
      BOOST_CHECK_EQUAL(md_recv.high_price(), md_orig.high_price());
      BOOST_CHECK_EQUAL(md_recv.low_price(), md_orig.low_price());
      BOOST_CHECK_EQUAL(md_recv.close_price(), md_orig.close_price());
      BOOST_CHECK_EQUAL(md_recv.volume(), md_orig.volume());
      BOOST_CHECK_EQUAL(md_recv.ask(), md_orig.ask());
      BOOST_CHECK_EQUAL(md_recv.bid(), md_orig.bid());
  }

}

void multicast_communication::tests_::test_minute_message_receiver::minute_message(
						       const minute_message_data& m)
{
  data_.clear();
  for (size_t i = 0; i < m.size(); ++i)
  {
    const minute_datafeed& md = m[i];
    data_[md.get_stock_name()] = md;
  }
}

std::string multicast_communication::tests_::timestamp_as_string(
    boost::uint8_t h, boost::uint8_t m, boost::uint8_t s, boost::uint16_t ms)
{
  char hour_code = h + '0';
  char minute_code = m + '0';
  char second_code = s + '0';
  
  std::stringstream ss;
  ss << hour_code << minute_code << second_code
     << std::setw(3) << std::setfill('0') << ms;
  return ss.str();
}

multicast_communication::quote_message_ptr multicast_communication::tests_::generate_qmessage(
            const std::string& secsymbol, boost::uint8_t h, boost::uint8_t m, boost::uint8_t s)
{
    boost::uint16_t ms = ::rand() % 1000;
    std::string timestamp = timestamp_as_string(h, m, s, ms);

    double bid_volume = ::rand();
    double offer_volume = ::rand();
 
    quote_message_ptr qmessage(new quote_message(timestamp, secsymbol, 0.0, bid_volume, 0.0, offer_volume));
    return qmessage;
}
multicast_communication::trade_message_ptr multicast_communication::tests_::generate_tmessage(
            const std::string& secsymbol, boost::uint8_t h, boost::uint8_t m, boost::uint8_t s)
{
    boost::uint16_t ms = ::rand() % 1000;
    std::string timestamp = timestamp_as_string(h, m, s, ms);

    double volume = ::rand();
    double price = ::rand();
 
    trade_message_ptr tmessage(new trade_message(timestamp, secsymbol, price, volume));
    return tmessage;
}

void multicast_communication::tests_::register_message(
    std::map<std::string, minute_datafeed>& data, quote_message_ptr& m)
{
  minute_datafeed& md = data[m->security_symbol()];
  md.set_bid(md.bid() + m->bid_price());
  md.set_ask(md.ask() + m->offer_price());
}
void multicast_communication::tests_::register_message(
    std::map<std::string, minute_datafeed>& data, trade_message_ptr& m)
{
  minute_datafeed& md = data[m->security_symbol()];
  if (md.open_price() == 0.0)
    md.set_open_price(m->price());
  if (md.high_price() < m->price())
    md.set_high_price(m->price());
  if (md.low_price() == 0.0 || md.low_price() > m->price())
    md.set_low_price(m->price());
  // close price refresh everytime
  md.set_close_price(m->price());

  md.set_volume(md.volume() + m->volume());
}


