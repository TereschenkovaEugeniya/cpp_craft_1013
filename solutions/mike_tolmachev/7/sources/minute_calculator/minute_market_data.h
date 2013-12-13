#pragma once

#include "market_data_processor.h"
#include "market_minute_processor.h"

#include "minute_calculator.h"
#include "market_data_receiver.h"


namespace minute_calculator
{

using namespace multicast_communication;

class minute_market_data : public market_data_processor,
							public market_minute_processor
{
	minute_calculator calculator_;
	multicast_communication::market_data_receiver receiver_;

	mutable boost::mutex mtx_;

	task5_6::thread_safe_queue<minute_data> minutes_;
    std::map<std::string, std::ofstream> outputs_;

	std::string output_dir_;

    boost::thread writer_;
    bool working_;

public:
    explicit minute_market_data(const size_t trade_thread_size,
								const size_t quote_thread_size,
								const ports_vector& trade_ports,
								const ports_vector& quote_ports,
                                const std::string& output_dir);
	explicit minute_market_data(const std::string& config_file, const std::string& output_dir);
	virtual ~minute_market_data();

    void stop();
	
private:
	virtual void new_trade( const trade_message_ptr& trade);
	virtual void new_quote( const quote_message_ptr& quote);

	virtual void new_minute(const minute_data& minute);

    void process_minutes();
};

}