#ifndef _MULTICAST_COMMUNICATION_COMMUNICATION_H_
#define _MULTICAST_COMMUNICATION_COMMUNICATION_H_

#include "market_data_processor.h"
#include "udp_listener.h"
#include "thread_safe_queue.h"

#include <vector>

#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace multicast_communication
{
	typedef std::pair< std::string, unsigned short > address;

	namespace async_udp
	{
		class udp_listener;
		typedef boost::shared_ptr< udp_listener > udp_listener_ptr;
	}
	
	class communication : virtual private boost::noncopyable
	{	
		typedef boost::shared_ptr< boost::asio::io_service > service_ptr;

	private:
		boost::mutex interrupt_protector;
		const size_t trade_thread_size;
		const size_t quote_thread_size;
		const std::vector< address >& trade_ports;
		const std::vector< address >& quote_ports;
		std::vector< std::pair< service_ptr, async_udp::udp_listener_ptr > >connections;

		boost::thread_group quote_threads, trade_threads;
		thread_safe_queue< std::string > trade_queue, quote_queue;

	public:
		processor_ptr processor;
		bool interrupt;
		boost::thread_group connection_threads;
	
		explicit communication 
			(	const size_t trade_thread_size,
				const size_t quote_thread_size,
				const std::vector< address >& trade_ports,
				const std::vector< address >& quote_ports,
				processor_ptr processor );

		virtual ~communication();

		void start();
		void new_connection( service_ptr io );
		void new_message( const std::string&, message_type );
		void process_trade();
		void process_quote();
	};
}


#endif // _MULTICAST_COMMUNICATION_COMMUNICATION_H_