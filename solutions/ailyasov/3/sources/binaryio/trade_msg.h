#pragma once

#include <vector>
#include <string>
#include <vector>
#include <iostream>

#include <boost/cstdint.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "reader.h"
#include "writer.h"

using namespace binaryio;
using boost::uint32_t;

enum TYPES {
    MARKET_OPEN = 1,
    TRADE = 2,
    QUOTE = 3,
    MARKET_CLOSE = 4
}; 

class TradeMsg {
    public:
        TradeMsg() {}
        TradeMsg(const uint32_t type, 
                const uint32_t time, 
                const uint32_t len, 
                const std::string msg) : type_(type), time_(time), len_(len), msg_(msg) { }

        friend std::ostream& operator<<(std::ostream& os, const TradeMsg& msg);
        static bool read(TradeMsg&, Reader&);
        static void write(std::vector<TradeMsg> const& messages, Writer& out);
        uint32_t get_type() const { return type_; }
        uint32_t get_time() const { return time_; }
        uint32_t get_len() const { return len_; }
        size_t get_size() const { return sizeof(uint32_t) * 3 + len_; }
        std::string get_msg() const { return msg_; } 
    private:
        uint32_t type_;
        uint32_t time_;
        uint32_t len_;
        std::string msg_;
}; 
