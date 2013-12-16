#ifndef _MULTICAST_COMMUNICATION_MARKET_DATA_PROCESSOR_H_
#define _MULTICAST_COMMUNICATION_MARKET_DATA_PROCESSOR_H_

#include <trade_message.h>
#include <quote_message.h>

namespace multicast_communication
{
	class market_data_processor
	{
		friend class market_data_receiver; // please insert here your processing class 
	public:
		explicit market_data_processor(){}
		virtual ~market_data_processor(){}
        template <typename message_ptr>
        void process(const message_ptr& mess);
	private:
		virtual void new_trade( const trade_message_ptr& ) = 0;
		virtual void new_quote( const quote_message_ptr& ) = 0;
	};

    template <>
    void market_data_processor::process< quote_message_ptr >(const quote_message_ptr& mess)
    {
        new_quote(mess);
    }
    
    template <>
    void market_data_processor::process< trade_message_ptr >(const trade_message_ptr& mess)
    {
        new_trade(mess);
    }
    
    
    

    
}

#endif // _MULTICAST_COMMUNICATION_MARKET_DATA_PROCESSOR_H_
