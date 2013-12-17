#include "test_registrator.h"
#include <minute_market_data.h>

#include <iostream>
#include <map>
#include <string>

//#include <boost/filesystem.hpp>

namespace multicast_communication
{
  namespace tests_
  {
    void on_stop();
    void on_minute_message(const minute_message_data& m);

    std::string fname_from_stockname(const std::string& stock_name);

    typedef std::map<std::string, boost::shared_ptr<std::ofstream> > results_type;
    results_type results;
  }
}


void multicast_communication::tests_::minute_market_data_tests()
{
  BOOST_CHECK_NO_THROW
  (
    minute_market_data mmd;
  )

  minute_market_data_ptr mmd_p(new minute_market_data());

  mmd_p->set_stop_handler(on_stop);
  mmd_p->set_minute_message_handler(on_minute_message);

  // it isn't completed yet ...
}

void multicast_communication::tests_::on_stop()
{
  results_type::iterator it = results.begin();
  while (it != results.end())
  {
    (*it).second->close();
    ++it;
  }
}
void multicast_communication::tests_::on_minute_message(
                            const minute_message_data& m)
{
  for (size_t i = 0; i < m.size(); ++i)
  {
    const minute_datafeed& md = m[i];
    boost::shared_ptr<std::ofstream> ofs_p;
    results_type::iterator it = results.find(md.get_stock_name());
    if (it != results.end())
      ofs_p = it->second;
    else
    {
      std::string fname = fname_from_stockname(md.get_stock_name());
      ofs_p.reset(new std::ofstream());
      ofs_p->open(fname.c_str(), std::ios::binary);
      if (ofs_p->is_open())
        results.insert(std::make_pair(md.get_stock_name(), ofs_p));
      else
      {
        std::cerr << "Can't open file " << fname << std::endl;
        continue;
      }
    } // if (it == results.end())
    md.write_to(*ofs_p);
  }
}

inline std::string multicast_communication::tests_::fname_from_stockname(
                                            const std::string& stock_name)
{
  std::string fname(stock_name);
  fname.append(".dat");
  return fname;
}
