#include "config.h"

#include <fstream>

template<typename MapContainer>
void read_map(std::istream& in, MapContainer& container)
{
    size_t size = 0;
    in >> size;
    container.resize(size);
    for(size_t iter = 0; iter < size; ++iter)
    {
        in >> container[iter].first >> container[iter].second;
    }
}

multicast_communication::config::config(const std::string& file_name)
    : trade_thread_size_(0), quote_thread_size_(0)
{
	std::ifstream conf(file_name);
    if(!conf.is_open())
    {
        return;
    }

    conf >> trade_thread_size_ >> quote_thread_size_;
	
	read_map( conf, trade_ports_);
    read_map( conf, quote_ports_);
}
