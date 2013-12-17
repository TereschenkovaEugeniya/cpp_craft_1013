#include <vector>
#include <algorithm>
#include <iterator>
#include "reader.h"
#include "writer.h"
#include "msg.h"
#include "small_buffer.h"

using binary_reader::Reader;
using binary_writer::Writer;

void SmallBuffer::read_to_buff_and_save_stats(const std::string& input, const std::string&
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

    MsgBuf buf;
    buf.read(in);

#ifndef NDEBUG
    const MsgTypeMap& map = buf.get_type_map();
    const std::vector<Msg>& messages = buf.get_messages();
    std::copy(messages.begin(), messages.end(),std::ostream_iterator<Msg>(std::cout, "\n"));

    std::cout << std::endl;

    for(MsgTypeMap::const_iterator it = map.begin(); it != map.end(); ++it) {
        std::cout << "[" << it->first << ", " << it->second << "]" << std::endl;
    }
#endif

    buf.write_type_map(out);
}

