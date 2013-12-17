#ifndef _BINARY_READER_READER_H_
#define _BINARY_READER_READER_H_

#include <fstream>
#include <stdint.h>

namespace binary_reader
{

struct Msg
{
    uint32_t type;
    uint32_t time;
    uint32_t len;
};

std::istream& operator >> (std::istream& in, Msg& msg);
std::ostream& operator << (std::ostream& out, const Msg& msg);

}

#endif // _BINARY_READER_READER_H_
