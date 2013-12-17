#pragma once

#include <string>
#include <vector>
#include "trade_msg.h"
#include "trade_msg_out.h"

class BinaryDatafeed {
    public:
        static void process(const std::string& input, const std::string&
                output);
    private:
        static const std::vector<TradeMsgOut> get_messages_for_output(const
                std::vector<TradeMsg>& msg);
        static TradeMsgOut msg_converter(const TradeMsg& m);
        BinaryDatafeed();
        BinaryDatafeed(const BinaryDatafeed&);
        void operator=(const BinaryDatafeed&); 
};
