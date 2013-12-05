#include "Message.h"

using namespace std;

Message::Message(const Message& o) 
  : type_(o.type_), time_(o.time_), 
    text_length_(o.text_length_), text_(NULL) {
  if (text_length_ > 0) {
    text_ = new char[text_length_];
    ::memcpy(text_, o.text_, text_length_);
  }
}
const Message& Message::operator=(const Message& o) {
  if (this != &o) {
    type_ = o.type_;
    time_ = o.time_;
    text_length_ = o.text_length_;
    delete [] text_;
    text_ = NULL;
    if (text_length_ > 0) {
      text_ = new char[text_length_];
      ::memcpy(text_, o.text_, text_length_);
    }
  }
  return (*this);
}

void Message::ReadFrom(istream& is) {
  is.read(reinterpret_cast<char*>(&type_), sizeof(type_));
  is.read(reinterpret_cast<char*>(&time_), sizeof(time_));
  is.read(reinterpret_cast<char*>(&text_length_), sizeof(text_length_));
  delete [] text_;
  text_ = NULL;
  if (text_length_ > 0) {
    text_ = new char[text_length_];
    memset(text_, 0, text_length_);
    is.read(text_, text_length_);
  }
}
void Message::WriteTo(ostream& os) const {
  os.write(reinterpret_cast<const char*>(&type_), sizeof(type_));
  os.write(reinterpret_cast<const char*>(&time_), sizeof(time_));
  os.write(reinterpret_cast<const char*>(&text_length_), sizeof(text_length_));
  if (text_length_ > 0)
    os.write(text_, text_length_);
}
