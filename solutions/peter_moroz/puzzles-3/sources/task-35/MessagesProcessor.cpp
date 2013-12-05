#include <fstream>
#include <sstream>
#include "Restrictions.h"
#include "MessagesProcessor.h"
#include "ThreadsafeStatistics.h"

using namespace std;

MessagesProcessor::MessagesProcessor() : statistics_(NULL) {}
MessagesProcessor::~MessagesProcessor() {}

void MessagesProcessor::Workflow() {
  stringstream msg;
  ifstream ifs(in_fname_.c_str(), ios_base::binary);
  if (!ifs.is_open())
    return;

  Message message;
  message.ReadFrom(ifs);

  boost::uint32_t start_time = message.time();
  boost::uint32_t current_time = start_time;

  if (statistics_ != NULL)
    statistics_->CheckAndCorrectTimeRange(message.time());
  messages_buffer_.Clear();

  while(!ifs.eof()) {
    if (message.type() <= restrictions::kMaximalAllowedMessageType) {

      if (statistics_ != NULL)
        statistics_->CheckAndCorrectTimeRange(message.time());

      if(message.time() > current_time) {
        messages_buffer_.Flush();
        current_time = message.time();
      }

      if (messages_buffer_.Accept(message)) {
        if (statistics_ != NULL)
          statistics_->IncrementCounter(message.type());
      }
      message.ReadFrom(ifs);
    } // if (message.type() <= kMaximalAllowedMessageType)
  } // while(!ifs.eof())
}
