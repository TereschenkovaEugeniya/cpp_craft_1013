#include "message.h"

binary_reader::Message::~Message()
{
    delete[] msg_;
}

void binary_reader::Message::read(std::fstream &in)
{
    in.read( reinterpret_cast<char*>(&type_), sizeof(boost::uint32_t) );
    in.read( reinterpret_cast<char*>(&time_), sizeof(boost::uint32_t) );
    in.read( reinterpret_cast<char*>(&len_), sizeof(boost::uint32_t) );


    msg_ = new char[len_];
    in.read( msg_, len_ );
}

void binary_reader::Message::write(std::fstream &out)
{
    out.write( reinterpret_cast<char*>(&type_), sizeof(boost::uint32_t) );
    out.write( reinterpret_cast<char*>(&time_), sizeof(boost::uint32_t) );
    out.write( reinterpret_cast<char*>(&len_), sizeof(boost::uint32_t) );

    out.write( msg_, len_ );
}

boost::uint32_t binary_reader::Message::getTime() const
{
    return time_;
}

boost::uint32_t binary_reader::Message::getType() const
{
    return type_;
}

boost::uint32_t binary_reader::Message::size() const
{
    return ( sizeof(time_) + sizeof(len_) + sizeof(type_) + sizeof(char)*len_ );
}

