#ifndef _MARKET_DATA_RECEIVER_H_
#define _MARKET_DATA_RECEIVER_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <string>
#include "ini_config.h"
#include "udp_listener.h"
#include "messages.h"

namespace multicast_communication
{
	class market_data_receiver : virtual private boost::noncopyable
	{
	public:
		market_data_receiver(const std::string& iniFileName);
		~market_data_receiver();
		void start();
	private:
		vector_address_port addr_port_QUOTE;
		vector_address_port addr_port_TRADE;
		size_t quote_thread_size;
		size_t trade_thread_size;

		std::vector < boost::shared_ptr < multicast_communication::udp_listener > > TradesListeners;
		std::vector < boost::shared_ptr < multicast_communication::udp_listener > > QuotesListeners;

		boost::asio::io_service io_service_trade, io_service_quote;
		boost::thread_group thread_group_trade, thread_group_quote;

		void run_service(boost::asio::io_service& io_serv);


	};
}
#endif // _MARKET_DATA_RECEIVER_H_

