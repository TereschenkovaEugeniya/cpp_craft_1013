#pragma once

#include <string>
#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "thread_safe_queue.h"

namespace multicast_communication
{
	//typedef boost::shared_ptr<task5_6::thread_safe_queue<std::string> > safe_queue_ptr;

	class market_data_connector : private boost::noncopyable
	{
		boost::asio::io_service& io_service_;

		boost::asio::ip::udp::endpoint listen_endpoint_;
		boost::asio::ip::udp::socket socket_;

		std::string multicast_address_;

		//safe_queue_ptr messages_;
		task5_6::thread_safe_queue<std::string>& messages_;

	public:
		explicit market_data_connector(boost::asio::io_service& io_service, const std::string& multicast_address, unsigned short port, task5_6::thread_safe_queue<std::string>& messages/*safe_queue_ptr& messages*/);
		~market_data_connector();
	private:
		void socket_reload_();
		void register_listen_();
		void listen_handler_(boost::shared_ptr<std::string> buf, const boost::system::error_code& error, const size_t bytes_received);
	};
}

