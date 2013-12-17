#include <string>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <vector>
#include <map>
#include <cctype>
#include <iterator>
#include <cstdio>

#include <boost/cstdint.hpp>

#include "binary_datafeed.h" 
#include "msg.h"
#include "msg_out.h"

using namespace binaryio;

void BinaryDatafeed::process(std::string const& input, std::string const& output) {
    Reader in(input);

    if(!in.is_open()) 
    {
        std::cerr << "Cannot open file " << input << std::endl;
        exit(0);
    }

    const std::vector<TradeMsg> msgs = TradeMsg::read(in);
    std::vector<TradeMsgOut> msgs_out;
    std::transform(msgs.begin(), msgs.end(), std::back_inserter(msgs_out), BinaryDatafeed::msg_converter);
    std::map<std::string, std::vector<TradeMsgOut> > m;
    for(std::vector<TradeMsgOut>::iterator it = msgs_out.begin(); it !=
            msgs_out.end(); ++it)
    {
        TradeMsgOut msg = *it;
        m[msg.get_stock_name()].push_back(msg);
    }
    for(std::map<std::string, std::vector<TradeMsgOut> >::iterator it =
            m.begin(); it != m.end(); ++it)
    {
        std::pair<std::string, std::vector<TradeMsgOut> > p = *it;
        std::string out_file(output);
        out_file.resize(output.size() + p.first.size() + 1);
        out_file.insert(out_file.find_last_of("."), "_" + p.first);
        Writer out(out_file);
        if(out.is_open()) 
        {
            TradeMsgOut::write(msgs_out, out); 
        } else 
        { 
            std::cerr << "Cannot open file " << output << std::endl;
        }
    }

#ifndef NDEBUG
    std::copy(msgs.begin(), msgs.end(), std::ostream_iterator<TradeMsg>(std::cout, "\n"));
    std::cout << std::endl;
    std::copy(msgs_out.begin(), msgs_out.end(), std::ostream_iterator<TradeMsgOut>(std::cout, "\n"));
#endif
}

TradeMsgOut BinaryDatafeed::msg_converter(TradeMsg const& m) {
    int day, month, year;
    std::sscanf(m.get_date_time().c_str(), "%4d%2d%2d", &year, &month, &day);
    uint32_t date = day + (month - 1) * 31 + 372 * (year - 1);

    return TradeMsgOut(m.get_stock_name(), date, m.get_price(), m.get_volume(),
            m.get_f2());
} 
