#include <conio.h>
#include <iostream>
//#include <functional>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "minute_market_data.h"

using namespace multicast_communication;

class puzzles7_application : public boost::noncopyable
{
public:
  puzzles7_application();

  void run();

public:
  void on_stop();
  void on_minute_message(const minute_message_data& m);

private:
  void print_user_hints();
  void translate_command(int cmd);

private:
  bool running_;
  minute_market_data_ptr mmd_p_;
};

int main()
{
  puzzles7_application app;
  app.run();

  return 0;
}

puzzles7_application::puzzles7_application() : running_(false)
{
  mmd_p_.reset(new minute_market_data());
}

void puzzles7_application::run()
{
  /*mmd_.set_stop_handler(
    std::bind1st(std::mem_fun(&puzzles7_application::on_stop), this));
  mmd_.set_minute_message_handler(
    std::bind1st(std::mem_fun(&puzzles7_application::on_minute_message), this));*/

  mmd_p_->set_stop_handler(boost::bind(&puzzles7_application::on_stop, this));
  mmd_p_->set_minute_message_handler(
             boost::bind(&puzzles7_application::on_minute_message, this, _1));

  if (!mmd_p_->start())
  {
    std::cerr << "Error: puzzles7_application::run() failed. "
              << "Reason: Can't start  minute_market_data instance\n";
    return;
  }

  running_ = true;
  print_user_hints();
  while (running_)
  {
    // I know, that it isn't portable, but still so...
    int key = _getch();
    translate_command(key);
  }
}

void puzzles7_application::on_stop()
{
  // does nothing, but I've left it for any case...
}
void puzzles7_application::on_minute_message(const minute_message_data& m)
{
  for (size_t i = 0; i < m.size(); ++i)
  {
    const minute_datafeed& md = m[i];
    std::string fname(md.get_stock_name());
    fname.append(".dat");
    std::ofstream ofs(fname.c_str(), std::ios::binary | std::ios::app);
    if (!ofs.is_open())
      std::cerr << "Can't open file " << fname << std::endl;
    md.write_to(ofs);
  }
}

void puzzles7_application::print_user_hints()
{
  std::cout << "Press [Ctrl] + [C] to stop\n";
}
void puzzles7_application::translate_command(int cmd)
{
  if (cmd == 0x03)
  {
    std::cout << "Application finished ... \n";
    running_ = false;
  }
}
