#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>
#include <boost/cstdint.hpp>

typedef boost::uint32_t MessageType;

class Message {
public:
  enum Types {
    MT_MarketOpen = 1,
    MT_Trade,
    MT_Quote,
    MT_MarketClose
  };

public:
  Message() : type_(0), time_(0), text_length_(0), text_(NULL) {}
  virtual ~Message() { delete [] text_; }

  Message(const Message& o);
  const Message& operator=(const Message& o);

  boost::uint32_t type() const { return type_; }
  boost::uint32_t time() const { return time_; }

  size_t GetTotalSize() const {
    return (sizeof(type_) + sizeof(time_) 
		+ sizeof(text_length_) + text_length_);
  }
  void ReadFrom(std::istream& is);
  void WriteTo(std::ostream& os) const;

private:
  boost::uint32_t type_;
  boost::uint32_t time_;
  boost::uint32_t text_length_;
  char* text_;
};

#endif // MESSAGE_H_
