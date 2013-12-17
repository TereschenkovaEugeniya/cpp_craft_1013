#include <stdexcept>
#include <fstream>
#include <iostream>
#include <sstream>
#include "MessagesContainer.h"

using namespace std;

static const char* kOutputFilePrefix = "Output_";
unsigned MessagesContainer::instances_counter_ = 0;

void MessagesContainer::DumpContent() const {
  ofstream ofs;
  string fname(kOutputFilePrefix);
  fname.append(name_);
  fname.append(".txt");
  ofs.open(fname.c_str(), ios_base::binary);
  if (!ofs.is_open()) {
    stringstream msg;
    msg << "Can't  open file: " << fname;
    throw runtime_error(msg.str());
  }

  vector<Message>::const_iterator it = messages_.begin();
  while (it != messages_.end()) {
    it->WriteTo(ofs);
    ++it;
  }
}
