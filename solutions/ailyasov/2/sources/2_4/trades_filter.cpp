#include <vector>
#include <algorithm>
#include <iterator>
#include "reader.h"
#include "writer.h"
#include "trade_msg.h"
#include "trades_filter.h"

using binary_reader::Reader;
using binary_writer::Writer;

void TradesFilter::filter_trades_and_save(const std::string& input, const std::string&
        output) {
    Reader in(input);
    if(!in.is_open()) 
    {
        std::cerr << "Cannot open file " << input << std::endl;
        exit(0);
    }
    Writer out(output);
    if(!out.is_open()) 
    {
        std::cerr << "Cannot open file " << output << std::endl;
        exit(0);
    }
    std::vector<TradeMsg> messages = TradeMsg::read_data(in);
    TradeMsg::write_data(messages, out);	
#ifndef NDEBUG
    std::copy(messages.begin(), messages.end(), std::ostream_iterator<TradeMsg>(std::cout, "\n"));
#endif
} 
