#include "message.h"


std::istream& operator >> (std::istream& input, message& m)
{
    read(input, &m.type);
    read(input, &m.time);
    read(input, &m.len);
    return input;
}

std::ostream& operator << (std::ostream& output, message& m)
{
    write(output, &m.type);
    write(output, &m.time);
    write(output, &m.len);
    return output;
}