#pragma once

#include "market_data_connector.h"
#include "market_data_processor.h"
#include <fstream>


namespace multicast_communication
{
    typedef std::vector<std::pair<std::string, unsigned short> > ports_vector;

	class market_data_receiver : private boost::noncopyable
	{
		task5_6::thread_safe_queue<std::string> quotes_;
		task5_6::thread_safe_queue<std::string> trades_;

		boost::asio::io_service io_service_trade_;
		boost::asio::io_service io_service_quote_;

		std::list<boost::shared_ptr<market_data_connector> > connectors_;

		boost::thread_group work_threads_;

		market_data_processor& processor_;

		bool working_;

	public:
		explicit market_data_receiver(const size_t trade_thread_size,
										const size_t quote_thread_size,
										const ports_vector& trade_ports,
										const ports_vector& quote_ports,
										market_data_processor& processor);

		explicit market_data_receiver(const std::string& file_name,
										market_data_processor& processor);

		~market_data_receiver();

		void stop();

	private:
		void service_thread(boost::asio::io_service& service);
		void trades_thread();
		void quotes_thread();

		void initialize(const size_t trade_thread_size,
						const size_t quote_thread_size,
						const ports_vector& trade_ports,
						const ports_vector& quote_ports);
	};
}