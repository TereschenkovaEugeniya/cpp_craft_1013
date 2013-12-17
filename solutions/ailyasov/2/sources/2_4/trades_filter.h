#pragma once

#include <string>
#include "trade_msg.h"

struct TradesFilter {
    public:
        static void filter_trades_and_save(const std::string&, const std::string&); 
    private: 
        TradesFilter();
        TradesFilter(const TradesFilter&);
        void operator=(const TradesFilter&);
        static bool cmp_by_time(const TradeMsg& msg1, const TradeMsg& msg2) {
            return msg1.get_time() < msg2.get_time();
        }
};
