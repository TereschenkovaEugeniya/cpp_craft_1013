#include <string>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <iterator>
#include <cstdio>

#include <boost/cstdint.hpp>

#include "binary_datafeed.h"
#include "trade_msg.h"
#include "trade_msg_out.h"

using binary_reader::Reader;
using binary_writer::Writer;

void BinaryDatafeed::process(const std::string& input, const std::string&
        output) {
    Reader in(input);
    Writer out(output);

    if(!in.is_open()) 
    {
        std::cerr << "Cannot open file " << input << std::endl;
        exit(0);
    }

    if(!out.is_open()) 
    {
        std::cerr << "Cannot open file " << output << std::endl;
        exit(0);
    }

    const std::vector<TradeMsg> messages = TradeMsg::read(in);
    std::vector<TradeMsgOut> messages_out = get_messages_for_output(messages);
    TradeMsgOut::write(messages_out, out);

#ifndef NDEBUG
    std::copy(messages.begin(), messages.end(), std::ostream_iterator<TradeMsg>(std::cout, "\n"));
    std::cout << std::endl;
    std::copy(messages_out.begin(), messages_out.end(), std::ostream_iterator<TradeMsgOut>(std::cout, "\n"));
#endif
}

TradeMsgOut BinaryDatafeed::msg_converter(const TradeMsg& m) {
    int day, month, year;
    std::sscanf(m.get_date_time().c_str(), "%4d%2d%2d", &year, &month, &day);
    uint32_t date = day + (month - 1) * 31 + 372 * (year - 1);

    return TradeMsgOut(m.get_stock_name(), date, m.get_price(), m.get_volume(),
            m.get_f2());
}

const std::vector<TradeMsgOut> BinaryDatafeed::get_messages_for_output(const std::vector<TradeMsg>& msg) {
    std::vector<TradeMsgOut> v;
    std::transform(msg.begin(), msg.end(), std::back_inserter(v),
            BinaryDatafeed::msg_converter);
    return v;
}
