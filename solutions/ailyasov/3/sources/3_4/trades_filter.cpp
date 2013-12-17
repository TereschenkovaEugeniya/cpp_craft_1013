#include <vector>
#include <algorithm>
#include <iterator>

#include "reader.h"
#include "writer.h"

#include <boost/cstdint.hpp>

#include "trade_msg.h"
#include "trades_filter.h"


void TradesFilter::filter_trades_and_save(
        std::string const& input, 
        std::string const& output ) {

    using namespace binaryio; 

    Writer out(output);
    if(!out.is_open()) 
    {
        std::cerr << "Cannot open output file " << output << std::endl;
        exit(0);
    }

    std::vector<TradeMsg> messages;
    uint32_t max_time = 0;
    TradeMsg msg;
	
    for( Reader in( input ); TradeMsg::read( msg, in ) ; )
    {
		
        if(in.is_open()) 
        {
            if(msg.get_type() <= MARKET_CLOSE && 
                    (max_time < 2 || msg.get_time() > std::max(max_time - 2, 0U)) )
            {
                max_time = std::max( max_time, msg.get_time() );
                messages.push_back( msg );
            }
        }
        else 
        {
            std::cerr << "Cannot open input file " << input << std::endl;
        }
		
    }	
    TradeMsg::write(messages, out);

#ifndef NDEBUG
    std::copy(messages.begin(), messages.end(), std::ostream_iterator<TradeMsg>(std::cout, "\n"));
#endif

} 
