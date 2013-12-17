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

class TradeMsg {
    public:
        TradeMsg(
                std::string stock_name,
                std::string date_time,
                double price,
                double vwap,
                uint32_t volume,
                double f1,
                double t1,
                double f2,
                double f3,
                double f4
                ) : 
            stock_name_(stock_name),
            date_time_(date_time),
            price_(price),
            vwap_(vwap),
            volume_(volume),
            f1_(f1),
            t1_(t1),
            f2_(f2),
            f3_(f3),
            f4_(f4)
    {
        //empty
    } 
        friend std::ostream& operator<<(std::ostream&, const TradeMsg&);
        static std::vector<TradeMsg> read(Reader&);
        std::string get_stock_name() const { return stock_name_; }
        std::string get_date_time() const { return date_time_; }
        double get_price() const { return price_; }
        double vwap() const { return vwap_; }
        uint32_t get_volume() const { return volume_; }
        double get_f1() const { return f1_; }
        double get_t1() const { return t1_; }
        double get_f2() const { return f2_; }
        double get_f3() const { return f3_; }
        double get_f4() const { return f4_; }
    private:
        TradeMsg();
        std::string stock_name_;
        std::string date_time_;
        double price_;
        double vwap_;
        uint32_t volume_;
        double f1_;
        double t1_;
        double f2_;
        double f3_;
        double f4_;
};
