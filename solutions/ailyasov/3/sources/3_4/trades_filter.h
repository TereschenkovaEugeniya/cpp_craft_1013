#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <functional>

#include <boost/cstdint.hpp>

#include "trade_msg.h"

class TradesFilter {
    public:
static void filter_trades_and_save( std::string const& input, std::string const& output);
    private: 
        TradesFilter();
        TradesFilter(TradesFilter const&);
        void operator=(TradesFilter const&);
        static bool cmp_by_time(TradeMsg const& msg1, TradeMsg const& msg2) {
            return msg1.get_time() < msg2.get_time();
        }
};

struct FilterPredicate : public std::unary_function<TradeMsg, std::vector<TradeMsg> >
{
        std::vector<TradeMsg> msgs;
        void operator()(TradeMsg const m)
        {
            static uint32_t max_time = 0;
            if(m.get_type() <= MARKET_CLOSE && (max_time < 2 || 
                        m.get_time() > std::max(max_time - 2, 0U)) )
            {
                max_time = m.get_time();
                msgs.push_back(m);
            }
        }
};
