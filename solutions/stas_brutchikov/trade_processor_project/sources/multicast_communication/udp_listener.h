#ifndef _ASYNC_UDP_UDP_LISTENER_H_
#define _ASYNC_UDP_UDP_LISTENER_H_

#include <string>
#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace async_udp
{
	class udp_listener : virtual private boost::noncopyable
	{
	public:
        typedef boost::function< void ( const std::string& ) > sink_type;

		explicit udp_listener( boost::asio::io_service& io_service, const std::string& multicast_address, unsigned short port , sink_type sink );
		~udp_listener();
	private:
        typedef boost::shared_ptr< std::string > buffer_type;
		void socket_reload_();
		void register_listen_(buffer_type lo_buffer = buffer_type());
		void listen_handler_(buffer_type lo_buffer, const boost::system::error_code& error, const size_t bytes_received );

		static const size_t default_buffer_size;

		boost::asio::io_service& io_service_;

		boost::asio::ip::udp::endpoint listen_endpoint_;
		boost::asio::ip::udp::socket socket_;

		std::string multicast_address_;


        
		sink_type sink_;
	};
}

#endif // _ASYNC_UDP_UDP_LISTENER_H_
