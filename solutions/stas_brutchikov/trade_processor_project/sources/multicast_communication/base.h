#pragma once
#include <vector>
#include <string>
#include <stdint.h>

#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>

namespace multicast_communication
{

enum message_type: uint32_t
{
    type_unknown,
	type_short,
	type_long
};


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
typedef std::vector < address_port > vector_address_port;

template<typename message_ptr>
message_ptr build( std::istream& in );

template<typename container_message_ptr, typename message_ptr>
bool parse( const std::string& data, container_message_ptr& obj )
{
    std::istringstream in( data );
    char c = in.get();
	if( c != SOH )
	{
		return false;
	}
    
    do
	{

        message_ptr message(build<message_ptr>(in));
        if(message->type() != type_unknown)
		{
			obj.push_back(message);
		}
		c = in.get();

		while(in)
		{
			if(c == US)
				break;
			else if(c == ETX)
				return true;

			c = in.get();
		}
	}while(in && c == US);

	return false;

}

} //namespace multicast_communication
