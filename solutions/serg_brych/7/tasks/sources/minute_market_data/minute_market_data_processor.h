#ifndef _MINUTE_MARKET_DATA_PROCESSOR_H_
#define _MINUTE_MARKET_DATA_PROCESSOR_H_

#include <iostream>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <map>
#include <fstream>

#include "minute_calculator_accumulator.h"
#include "market_data_processor.h"
#include "market_data_receiver.h"

static const std::string config_file_path = BINARY_DIR "/config.ini";

namespace minute_market_data
{

	class minute_market_data_processor : public multicast_communication::market_data_processor
	{
		typedef boost::shared_ptr<multicast_communication::market_data_receiver> market_data_receiver_ptr;
		typedef boost::shared_ptr<minute_calculator::minute_calculator_accumulator> minute_calculator_accumulator_ptr;
		boost::mutex mutex_;
		boost::mutex data_mutex_;
		boost::condition_variable cv_;
		market_data_receiver_ptr receive_;
		minute_calculator_accumulator_ptr calculator_;
		std::list<minute_calculator::container_minute_data_ptr> output_;
		std::map<std::string, std::ofstream> out_;
		virtual void new_trade( const multicast_communication::trade_message_ptr& );
		virtual void new_quote( const multicast_communication::quote_message_ptr& );
		void disk_writer();
		bool stop_;
		boost::scoped_ptr<boost::thread> write_thread_ptr_;
	public:
		
		void run();
		void stop();
		explicit minute_market_data_processor();
		~minute_market_data_processor();

	};
}

#endif //_MINUTE_MARKET_DATA_PROCESSOR_H_