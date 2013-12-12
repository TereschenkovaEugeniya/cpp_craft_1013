#include "test_registrator.h"
#include <minute_datafeed.h>

#include <boost/filesystem.hpp>

namespace multicast_communication
{
  namespace tests_
  {
    void fill_randomly_minute_data(minute_datafeed& md);
  }
}


void multicast_communication::tests_::minute_datafeed_tests()
{
  BOOST_CHECK_NO_THROW
  (
    minute_datafeed md; 
  )

  minute_datafeed md_x;
  minute_datafeed md_y;

  fill_randomly_minute_data(md_x);
  md_y = md_x;

  BOOST_CHECK_EQUAL(md_x.minute(), md_y.minute());
  BOOST_CHECK_EQUAL(md_x.get_stock_name(), md_y.get_stock_name());
  BOOST_CHECK_EQUAL(md_x.open_price(), md_y.open_price());
  BOOST_CHECK_EQUAL(md_x.high_price(), md_y.high_price());
  BOOST_CHECK_EQUAL(md_x.low_price(), md_y.low_price());
  BOOST_CHECK_EQUAL(md_x.close_price(), md_y.close_price());
  BOOST_CHECK_EQUAL(md_x.volume(), md_y.volume());
  BOOST_CHECK_EQUAL(md_x.bid(), md_y.bid());
  BOOST_CHECK_EQUAL(md_x.ask(), md_y.ask());


  const size_t kRW_LoopCount = 16;
  for (size_t i = 0; i < kRW_LoopCount; ++i)
  {
    const char* kFilename = "minute_data.dat";

    std::ofstream ofs( kFilename, std::ios::binary );
    md_x.write_to(ofs);
    ofs.close();

    std::ifstream ifs( kFilename, std::ios::binary );
    md_y.read_from(ifs);
    ifs.close();

    boost::filesystem::remove( kFilename );    

    BOOST_CHECK_EQUAL(md_x.minute(), md_y.minute());
    BOOST_CHECK_EQUAL(md_x.get_stock_name(), md_y.get_stock_name());
    BOOST_CHECK_EQUAL(md_x.open_price(), md_y.open_price());
    BOOST_CHECK_EQUAL(md_x.high_price(), md_y.high_price());
    BOOST_CHECK_EQUAL(md_x.low_price(), md_y.low_price());
    BOOST_CHECK_EQUAL(md_x.close_price(), md_y.close_price());
    BOOST_CHECK_EQUAL(md_x.volume(), md_y.volume());
    BOOST_CHECK_EQUAL(md_x.bid(), md_y.bid());
    BOOST_CHECK_EQUAL(md_x.ask(), md_y.ask());
  }
}

void multicast_communication::tests_::fill_randomly_minute_data(minute_datafeed& md)
{
  static bool initialized = false;
  if (!initialized)
  {
    ::srand(::time(NULL));
    initialized = true;
  }

  boost::uint32_t minute = ::rand();
  std::string stock_name = (::rand() % 2) ? "AAPL" : "GOOG";
  double open_price = ::rand();
  double high_price = ::rand();
  double low_price = ::rand();
  double close_price = ::rand();
  double volume = ::rand();
  double bid = ::rand();
  double ask = ::rand();

  md.set_minute(minute);
  md.set_stock_name(stock_name.c_str());
  md.set_open_price(open_price);
  md.set_high_price(high_price);
  md.set_low_price(low_price);
  md.set_bid(bid);
  md.set_ask(ask);
}
