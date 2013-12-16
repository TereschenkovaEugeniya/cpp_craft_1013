
#ifndef _MULTICAST_COMMUNICATION_H_
#define _MULTICAST_COMMUNICATION__H_

#include <vector>

#include <trade_message.h>
#include <quote_message.h>
#include <market_data_processor.h>

namespace multicast_communication
{

    typedef std::pair<std::string, unsigned short> address;
    typedef std::vector<address> address_list;

	class multicast_communication
	{

	public:
		explicit multicast_communication(
            const size_t trade_thread_size,
            const size_t quote_thread_size,
            const address_list& trade_ports,
            const address_list& qoute_ports,
            market_data_processor& processor) 
                : _trade_thread_size(trade_thread_size),
                _quote_thread_size(quote_thread_size),
                _trade_ports(trade_ports),
                _qoute_ports(qoute_ports),
                _processor(processor)
        {
        }

	private:
		const size_t _trade_thread_size;
        const size_t _quote_thread_size;
        const address_list& _trade_ports;
        const address_list& _qoute_ports;
        market_data_processor& _processor;
	};
}

#endif // _MULTICAST_COMMUNICATION__H_
