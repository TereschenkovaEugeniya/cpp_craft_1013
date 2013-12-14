#ifndef _MARKET_DATA_SENDER_H_
#define _MARKET_DATA_SENDER_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <string>
#include "ini_config.h"
#include "udp_sender.h"
#include "messages.h"

#define SampleTime 5000


namespace multicast_communication
{
	class market_data_sender : virtual private boost::noncopyable
	{
	public:
		market_data_sender(const std::string& iniFileName, const std::string& PathToData);
		~market_data_sender();
		void start();
		void stop();
	private:
		std::string Path;
		vector_address_port addr_port_QUOTE;
		vector_address_port addr_port_TRADE;
		size_t quote_thread_size;
		size_t trade_thread_size;

		std::vector < boost::shared_ptr < multicast_communication::sender > > TradesSenders;
		std::vector < boost::shared_ptr < multicast_communication::sender > > QuotesSenders;

		boost::asio::io_service io_service_trade, io_service_quote;
		boost::thread_group thread_group_trade, thread_group_quote;

		void run_service(boost::asio::io_service& io_serv);


	};
}
#endif // _MARKET_DATA_SENDER_H_

