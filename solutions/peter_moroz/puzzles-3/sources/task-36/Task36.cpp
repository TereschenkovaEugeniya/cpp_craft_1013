#include "Task36.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include "MessagesSorter.h"

using namespace std;

Task36::Task36(const char* input_filename) 
            : in_filename_(input_filename) {}
Task36::~Task36() { ifs_.close(); }


void Task36::Perform() {
  if (!ifs_.is_open()) {
    stringstream message;
    message << "Task36::Perform() : class instance hasn't been "
      << "initialized yet, invoke Initialize() before." << endl;
    throw logic_error(message.str());
  }

  MessagesSorter messages_sorter;

  Message message;
  message.ReadFrom(ifs_);
  while (!ifs_.eof()) {
    try {
      message.CalculateDays();
      messages_sorter.PutMessage(message);
    } catch (exception& ex) { 
      cerr << "Task36::Perform - exception has been caught " 
          << "reason: " << ex.what() << endl;
    }
    
    message.ReadFrom(ifs_);
  }

  messages_sorter.DumpMessages();
}
void Task36::Initialize() {
  ifs_.open(in_filename_.c_str(), ios_base::binary);
  if (!ifs_.is_open()) {
    stringstream msg;
	  msg << "Can't open file: " << in_filename_;
    throw logic_error(msg.str());
  }
}
