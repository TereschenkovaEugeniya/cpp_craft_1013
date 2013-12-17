#include <iostream>
#include <vector>
#include <cctype>
#include <iterator>
#include <cstddef>

#include "trade_msg.h"
#include "reader.h"

using namespace binaryio;

std::ostream& operator<<(std::ostream& os, TradeMsg const & msg) 
{
    os << "[" << msg.type_ << "," << msg.time_ << "," << msg.len_ << "," << msg.msg_ << "]";
    return os;
} 

bool TradeMsg::read(TradeMsg& msg, Reader& in) 
{
	if(in.is_open() && !in.eof())  
    {
        msg.type_ = in.get_binary<uint32_t>();
        msg.time_ = in.get_binary<uint32_t>();
        msg.len_ = in.get_binary<uint32_t>();		
		if(in.eof()) return false;
        msg.msg_ = in.get_string(msg.len_);		
        return true;    
	}
    return false;
} 

void TradeMsg::write(std::vector<TradeMsg> const& messages, Writer& out) {
    for(std::vector<TradeMsg>::const_iterator it = messages.begin(); it != messages.end(); ++it) {
        out.save_binary( (*it).get_type() );
        out.save_binary( (*it).get_time() );
        out.save_binary( (*it).get_len() );
        out.save_string( (*it).get_msg() );
    }
}
