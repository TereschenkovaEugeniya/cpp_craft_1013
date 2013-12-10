#pragma once

#include "market_data_processor.h"
#include <fstream>
#include <boost/thread.hpp>

namespace multicast_communication
{
	class market_data_processor_helper : public market_data_processor
	{
		mutable boost::mutex mtx;
		std::ofstream& output_;
	public:
		explicit market_data_processor_helper(std::ofstream& output);

		virtual ~market_data_processor_helper();
	private:
		virtual void new_trade( const trade_message_ptr& );
		virtual void new_quote( const quote_message_ptr& );
	};
}