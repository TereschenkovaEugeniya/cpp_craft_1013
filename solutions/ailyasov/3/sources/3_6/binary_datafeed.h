#pragma once

#include <string>
#include <vector>
#include "msg.h"
#include "msg_out.h"

class BinaryDatafeed {
    public:
        static void process(const std::string& input, std::string const& output);
    private:
        static TradeMsgOut msg_converter(const TradeMsg& m);
        BinaryDatafeed();
        BinaryDatafeed(const BinaryDatafeed&);
        void operator=(const BinaryDatafeed&); 
};
