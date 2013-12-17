#include "Task26.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include "Message.h"

using namespace std;

void Task26::Perform() {
  if (!ifs_.is_open() || !ofs_.is_open()) {
    stringstream message;
    message << "Task26::Perform() : class instance hasn't been "
      << "initialized yet, invoke Initialize() before." << endl;
    throw logic_error(message.str());
  }

  Message message;
  message.ReadFrom(ifs_);
  while (!ifs_.eof()) {
    try {
      message.CalculateDays();
      message.WriteTo(ofs_);
    } catch (const exception& ex) { 
      cerr << "Task26::Perform exception has been caught " 
          << "reason: " << ex.what() << endl;
    }
    
    message.ReadFrom(ifs_);
  }
}
void Task26::Initialize() {
  stringstream message;
  ifs_.open(input_fname().c_str(), ios_base::binary);
  if (!ifs_.is_open()) {
    message << "Task26::Initialize() : Can't open file "
      << input_fname() << endl;
    throw runtime_error(message.str());
  }

  ofs_.open(output_fname().c_str(), ios_base::binary);
  if (!ofs_.is_open()) {
    message << "Task26::Initialize() : Can't open file "
      << output_fname() << endl;
    throw runtime_error(message.str());
  }
}
