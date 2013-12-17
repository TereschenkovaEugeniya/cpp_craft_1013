#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "reader.h"
#include "writer.h"

#include <boost/cstdint.hpp>

using namespace binaryio;
//[ stock_name, date, price, volume, f2 ]
class TradeMsgOut {
    public:
        TradeMsgOut(
                std::string stock_name,
                uint32_t date,
                double price,
                uint32_t volume,
                double f2
                ) : 
            stock_name_(stock_name),
            date_(date),
            price_(price),
            volume_(volume),
            f2_(f2)
    {
        stock_name_.resize(9);
    } 
        TradeMsgOut() {
            //empty
        }
        friend std::ostream& operator<<(std::ostream&, const TradeMsgOut&);
        static void write(const std::vector<TradeMsgOut>, Writer&);
        std::string get_stock_name() const { return stock_name_; }
        uint32_t get_date() const { return date_; }
        double get_price() const { return price_; }
        uint32_t get_volume() const { return volume_; }
        double get_f2() const { return f2_; }
    private:
        std::string stock_name_;
        uint32_t date_;
        double price_;
        uint32_t volume_;
        double f2_;
};
