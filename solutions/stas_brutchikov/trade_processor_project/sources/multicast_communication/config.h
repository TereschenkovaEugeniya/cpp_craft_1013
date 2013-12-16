#pragma once

#include <boost/noncopyable.hpp>

#include "base.h"

namespace multicast_communication
{
	class config: public boost::noncopyable
	{
	public:
        explicit config(const std::string& file_name);
        
        const vector_address_port& trade_ports() const 
        {
            return trade_ports_;
        }

		const vector_address_port& quote_ports() const
        {
            return quote_ports_;
        }
        
		size_t trade_thread_size() const
        {
            return trade_thread_size_;
        }

		size_t quote_thread_size() const
        {
            return quote_thread_size_;
        }

	private:    
        vector_address_port trade_ports_;
		vector_address_port quote_ports_;

		size_t trade_thread_size_;
		size_t quote_thread_size_;
	};
}
