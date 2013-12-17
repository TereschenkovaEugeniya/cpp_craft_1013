#include "reader.h"

namespace binary_reader
{

std::istream& operator >> (std::istream& in, Msg& msg)
{
    in.read(reinterpret_cast<char*>(&msg.type), sizeof(uint32_t));
    in.read(reinterpret_cast<char*>(&msg.time), sizeof(uint32_t));
    in.read(reinterpret_cast<char*>(&msg.len), sizeof(uint32_t));
    return in;
}

std::ostream& operator << (std::ostream& out, const Msg& msg)
{
    out.write(reinterpret_cast<const char*>(&msg.type), sizeof(uint32_t));
    out.write(reinterpret_cast<const char*>(&msg.time), sizeof(uint32_t));
    out.write(reinterpret_cast<const char*>(&msg.len), sizeof(uint32_t));
    return out;
}

}