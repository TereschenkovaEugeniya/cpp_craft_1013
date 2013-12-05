#include "Task34.h"
#include "Message.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <boost/thread.hpp>
                                                                                               
using namespace std;

boost::mutex cout_guard;

inline bool IsAllowedMessageType(MessageType type) {
  return (type == Message::MT_MarketOpen ||
          type == Message::MT_Trade || 
          type == Message::MT_Quote || 
          type == Message::MT_MarketClose);
}

inline void PrintMessage(const string& msg) {
  boost::mutex::scoped_lock lock(cout_guard);
  cout << msg << endl;
}

void ProcessFile(const string& file_number) {
  stringstream msg;
  string in_fname = "Input_";
  in_fname.append(file_number);
  in_fname.append(".txt");
  string out_fname = "Output_";
  out_fname.append(file_number);
  out_fname.append(".txt");

  ifstream ifs(in_fname.c_str(), ios_base::binary);
  if (!ifs.is_open())
    return;

  ofstream ofs(out_fname.c_str(), ios_base::binary);
  if (!ofs.is_open())
    return;

  boost::uint32_t max_time = 0;
  Message message;
  message.ReadFrom(ifs);
  while (!ifs.eof()) {
    // filter messages here
    if (IsAllowedMessageType(message.type())) {
      if (message.time() > max_time)
        max_time = message.time();
      if (max_time < 2) {
        if (message.time() == max_time)
          message.WriteTo(ofs);
      } else {
        if (message.time() > (max_time - 2))
          message.WriteTo(ofs);
      } // if (max_time < 2)
    } // if (IsAllowedMessageType(message.type()))
    message.ReadFrom(ifs);
  }
}

void Task34::Perform() {

  cout << "Task34::Perform has started : "
    << "start_file_number = " << start_file_number_
    << " number_of_files = " << number_of_files_ << endl;

  unsigned finish_file_number = start_file_number_;
  finish_file_number += number_of_files_;

  boost::thread_group thg;
  unsigned curr_file_number = start_file_number_;
  while (curr_file_number < finish_file_number) {
    stringstream msg;
    stringstream file_number;

    file_number << setw(3) << setfill('0') << curr_file_number;
    ++curr_file_number;

    thg.create_thread(boost::bind(&ProcessFile, file_number.str()));
  }

  thg.join_all();

  cout << "Task34::Perform has finished.\n";
}
