#include "Task24.h"
#include <stdexcept>
#include <sstream>

using namespace std;

bool Task24::IsAllowedMessageType(MessageType type) {
  return (type == Message::MT_MarketOpen ||
          type == Message::MT_Trade || 
          type == Message::MT_Quote || 
          type == Message::MT_MarketClose);
}

void Task24::Perform() {
  if (!ifs_.is_open() || !ofs_.is_open()) {
    stringstream message;
    message << "Task24::Perform() : class instance hasn't been "
      << "initialized yet, invoke Initialize() before." << endl;
    throw logic_error(message.str());
  }

  boost::uint32_t max_time = 0;
  Message message;
  message.ReadFrom(ifs_);
  while (!ifs_.eof()) {
    // filter messages here
    if (IsAllowedMessageType(message.type())) {
      if (message.time() > max_time)
        max_time = message.time();
      if (max_time < 2) {
        if (message.time() == max_time)
          message.WriteTo(ofs_);
      } else {
        if (message.time() > (max_time - 2))
          message.WriteTo(ofs_);
      } // if (max_time < 2)
    } // if (IsAllowedMessageType(message.type()))
    message.ReadFrom(ifs_);
  }

}
void Task24::Initialize() {
  stringstream message;
  ifs_.open(input_fname().c_str(), ios_base::binary);
  if (!ifs_.is_open()) {
    message << "Task24::Initialize() : Can't open file "
      << input_fname() << endl;
    throw runtime_error(message.str());
  }

  ofs_.open(output_fname().c_str(), ios_base::binary);
  if (!ofs_.is_open()) {
    message << "Task24::Initialize() : Can't open file "
      << output_fname() << endl;
    throw runtime_error(message.str());
  }
}
