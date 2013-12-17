#include "Task25.h"
#include <stdexcept>
#include <sstream>

using namespace std;

void Task25::Perform() {
  stringstream error_text;
  if (!ifs_.is_open() || !ofs_.is_open()) {
    error_text << "Task25::Perform() : class instance hasn't been "
      << "initialized yet, invoke Initialize() before." << endl;
    throw logic_error(error_text.str());
  }


  Message message;
  message.ReadFrom(ifs_);

  boost::uint32_t start_time = message.time();
  boost::uint32_t current_time = start_time;

  statistics_.Reset();
  statistics_.set_start_time(start_time);

  messages_buffer_.Clear();

  while(!ifs_.eof()) {
    if (message.type() <= kMaximalAllowedMessageType) {
      if(message.time() > current_time) {
        messages_buffer_.Flush();
        current_time = message.time();
      }

      messages_buffer_.PutMessage(message);
      message.ReadFrom(ifs_);
    } // if (message.type() <= kMaximalAllowedMessageType)
  } // while(!ifs_.eof())

  try {
    statistics_.DumpOnTime(ofs_, message.time());
  } catch (const exception& ex) {
    error_text << "Task25::Perform() : exception has been caught. "
      << "Reason: " << ex.what() << endl;
    throw logic_error(error_text.str());
  }
}
void Task25::Initialize() {
  stringstream message;
  ifs_.open(input_fname().c_str(), ios_base::binary);
  if (!ifs_.is_open()) {
    message << "Task25::Initialize() : Can't open file "
      << input_fname() << endl;
    throw runtime_error(message.str());
  }

  ofs_.open(output_fname().c_str(), ios_base::binary);
  if (!ofs_.is_open()) {
    message << "Task25::Initialize() : Can't open file "
      << output_fname() << endl;
    throw runtime_error(message.str());
  }
}
