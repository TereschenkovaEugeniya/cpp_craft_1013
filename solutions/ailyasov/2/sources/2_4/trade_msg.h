#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <boost/cstdint.hpp>

#include "reader.h"
#include "writer.h"

using binary_reader::Reader;
using binary_writer::Writer;

using boost::uint32_t;

enum TYPES {
    MARKET_OPEN = 1,
    TRADE = 2,
    QUOTE = 3,
    MARKET_CLOSE = 4
};

class TradeMsg {
    public:
        TradeMsg(const uint32_t type, const uint32_t time, const
                uint32_t len, const std::string msg) : 
            type_(type), time_(time), len_(len), msg_(msg) {
                //empty
            } 
        TradeMsg() {
            //empty
        }
        friend std::ostream& operator<<(std::ostream&, const TradeMsg&);
        static std::vector<TradeMsg> read_data(Reader&);
        static void write_data(std::vector<TradeMsg>, Writer&);
        uint32_t get_type() const { return type_; }
        uint32_t get_time() const { return time_; }
        uint32_t get_len() const { return len_; }
        std::string get_msg() const { return msg_; }
        static uint32_t max_time;
    private:
        uint32_t type_;
        uint32_t time_;
        uint32_t len_;
        std::string msg_;
};
