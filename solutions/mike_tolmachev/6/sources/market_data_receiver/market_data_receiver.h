#ifndef _MULTICAST_COMMUNICATION_MARKET_DATA_RECEIVER_H_
#define _MULTICAST_COMMUNICATION_MARKET_DATA_RECEIVER_H_

#include <string>
#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace multicast_communication
{
	class market_data_receiver : private boost::noncopyable
	{
		boost::asio::io_service& io_service_;

		boost::asio::ip::udp::endpoint listen_endpoint_;
		boost::asio::ip::udp::socket socket_;

		std::string multicast_address_;

		mutable boost::mutex protect_messages_;
		std::vector< std::string > messages_;

	public:
		explicit market_data_receiver(boost::asio::io_service& io_service, const std::string& multicast_address, unsigned short port);
		~market_data_receiver();
		const std::vector<std::string> messages() const;
	private:
		void socket_reload_();
		void register_listen_();
		void listen_handler_(boost::shared_ptr<std::string> buf, const boost::system::error_code& error, const size_t bytes_received);
	};
}

#endif // _MULTICAST_COMMUNICATION_MARKET_DATA_RECEIVER_H_
