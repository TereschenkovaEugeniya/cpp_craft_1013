#ifndef _MARKET_DATA_RECEIVER_H_
#define _MARKET_DATA_RECEIVER_H_

#include "market_data_processor.h"
#include "config_reader.h"
#include "udp_listener.h"


#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <string>
#include <stdexcept>

static const std::string config_file("config.ini");

namespace multicast_communication
{
	class market_data_receiver : virtual private boost::noncopyable
	{
	private:
		boost::shared_ptr<config_reader> config_;
		boost::thread_group	process_threads_;
		
		market_data_processor& processor_;

		typedef std::shared_ptr< boost::asio::io_service > service_ptr;
		typedef std::shared_ptr< async_udp::udp_listener > udp_listner_ptr;
		
        std::vector< service_ptr > services_trades_;
		std::vector< service_ptr > services_quotes_;
		std::vector< udp_listner_ptr > udp_listners_trades;
		std::vector< udp_listner_ptr > udp_listners_quotes;

		void process_thread(service_ptr);

	public:
		explicit market_data_receiver(market_data_processor &);
		explicit market_data_receiver(market_data_processor &, config_reader* );
		~market_data_receiver();
		void run();
		void stop();

	};

}


#endif //_MARKET_DATA_RECEIVER_H_