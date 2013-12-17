#include <iostream>
#include <iomanip>
#include <vector>
#include <cctype>
#include <iterator>
#include <cstddef>

#include "trade_msg.h"
#include "reader.h"
#include "writer.h"

using std::vector;
using binary_reader::Reader;
using binary_writer::Writer;

std::ostream& operator<<(std::ostream& os, const TradeMsg& msg) {
    os << "[" << 
        msg.stock_name_ << "," << 
        msg.date_time_ << "," << std::fixed << std::setprecision(2) << 
        msg.price_ << "," << std::fixed << std::setprecision(2) << 
        msg.vwap_ << "," << 
        msg.volume_ << "," << std::fixed << std::setprecision(2) << 
        msg.f1_ << "," << std::fixed << std::setprecision(2) << 
        msg.t1_ <<"," << std::fixed << std::setprecision(2) << 
        msg.f2_ <<"," << std::fixed << std::setprecision(2) << 
        msg.f3_ <<"," << std::fixed << std::setprecision(2) << 
        msg.f4_ << "]";
    return os;
} 

vector<TradeMsg> TradeMsg::read(Reader& in) {
    vector<TradeMsg> messages;
    while(!in.eof()) {
        std::string stock_name = in.get_string(8);
        std::string date_time = in.get_string(8);
        double price = in.get_binary<double>();
        double vwap = in.get_binary<double>();
        uint32_t volume = in.get_binary<uint32_t>();
        double f1 = in.get_binary<double>();
        double t1 = in.get_binary<double>();
        double f2 = in.get_binary<double>();
        double f3 = in.get_binary<double>();
        double f4 = in.get_binary<double>();
        if(in.eof()) break; 
        TradeMsg msg(stock_name, date_time, price, vwap, volume, f1, t1, f2,
                f3 ,f4);
        messages.push_back(msg);
    }
    return messages;
} 
