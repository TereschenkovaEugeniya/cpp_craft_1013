#include "test_registrator.h"
#include <iomanip>
#include <sstream>

#include <trade_message.h>

void multicast_communication::tests_::trade_message_tests()
{
  ::srand(::time(NULL));

  BOOST_CHECK_NO_THROW
  (
    int h_limit = ::rand() % 24;
    int m_limit = ::rand() % 60;
    int s_limit = ::rand() % 60;

    for (int h = 0; h < h_limit; ++h)
    {
      char hour_code = h + '0';
      for (int m = 0; m < m_limit; ++m)
      {
        char minute_code = m + '0';
        for (int s = 0; s < s_limit; ++s)
        {
          char second_code = s + '0';
          for (int ms = 0; ms < 1000; ++ms)
          {
            std::stringstream ss;
            ss << hour_code << minute_code << second_code
              << std::setw(3) << std::setfill('0') << ms;

            std::string security_symbol;
            if (::rand() % 2)
              security_symbol.assign("GOOG");
            else
              security_symbol.assign("AAPL");

            double price = ::rand();
            double volume = ::rand();
            trade_message_ptr message(new trade_message(ss.str(), security_symbol, price, volume));

            boost::uint32_t reference_ts = (((h * 60 + m) * 60) + s) * 1000 + ms;

            BOOST_CHECK_EQUAL(reference_ts, message->get_timestamp());
            BOOST_CHECK_EQUAL(security_symbol, message->security_symbol() );
            BOOST_CHECK_EQUAL(price, message->price());
            BOOST_CHECK_EQUAL(volume, message->volume());
          }
        }
      }
    }
  )
}
