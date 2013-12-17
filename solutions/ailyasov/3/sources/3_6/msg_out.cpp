#include <iostream>
#include <vector>
#include <cctype>
#include <iterator>
#include <cstddef>

#include "msg.h"
#include "msg_out.h"
#include "reader.h"
#include "writer.h"

using namespace binaryio;

std::ostream& operator<<(std::ostream& os, const TradeMsgOut& msg) {
    os << "[" << 
        msg.stock_name_ << "," << 
        msg.date_ << "," << 
        msg.price_ << "," << 
        msg.volume_ << "," << 
        msg.f2_ << "]";
    return os;
} 

void TradeMsgOut::write(const std::vector<TradeMsgOut> messages, Writer& out) {
    for(std::vector<TradeMsgOut>::const_iterator it = messages.begin(); it != messages.end(); ++it) {
        out.save_string((*it).get_stock_name());
        out.save_binary((*it).get_date());
        out.save_binary((*it).get_price());
        out.save_binary((*it).get_volume());
        out.save_binary((*it).get_f2());
    }

}
