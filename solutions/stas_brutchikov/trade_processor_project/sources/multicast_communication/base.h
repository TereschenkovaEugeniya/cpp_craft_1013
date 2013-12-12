#pragma once
#include <vector>
#include <string>
#include <stdint.h>

#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>

namespace multicast_communication
{
static const std::map< const char, double > denominators = boost::assign::map_list_of
	('0' , 1.0) ('3' , 8.0) ('4' , 16.0) ('5' , 32.0) ('6' , 64.0) ('7' , 128.0) ('8' , 256.0)
    ('A' , 10.0) ('B' , 100.0) ('C' , 1000.0 ) ('D' , 10000.0 )
    ('E' , 100000.0 ) ('F' , 1000000.0 ) ('G' , 10000000.0  )
    ('H' , 100000000.0 ) ('I' , 1.0)
    ;


template<typename type, size_t read_size>
type get(std::istream& in)
{
    char buffer[ read_size ];
    in.read( buffer, read_size );
    return boost::lexical_cast< type >( buffer, read_size );
}

const char SOH = 0x01;
const char ETX = 0x03;
const char US = 0x1f;

typedef std::pair < std::string, unsigned short > address_port;
typedef std::vector < address_port > vector_adres_port;

template<typename T>
bool parse(const std::string& data, T& obj);

} //namespace multicast_communication
