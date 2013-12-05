#ifndef _DATA_PROCESSOR_H_
#define _DATA_PROCESSOR_H_

#include <iostream>

#include <boost/thread.hpp>

#include "market_data_processor.h"

namespace multicast_communication
{

	class data_processor: public market_data_processor
	{
		std::ostream &output_;
		boost::mutex mutex_;
	private:
		virtual void new_trade( const trade_message_ptr& );
		virtual void new_quote( const quote_message_ptr& );
	public:

		explicit data_processor(std::ostream&);
		~data_processor();
	};

}
#endif //_DATA_PROCESSOR_H_