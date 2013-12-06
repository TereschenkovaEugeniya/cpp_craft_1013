#ifndef _MULTICAST_COMMUNICATION_MARKET_DATA_RECEIVER_H_
#define _MULTICAST_COMMUNICATION_MARKET_DATA_RECEIVER_H_

#include <market_data_processor.h>

#include <boost/noncopyable.hpp>

namespace multicast_communication
{
	class market_data_receiver : virtual protected boost::noncopyable
	{
		market_data_processor& processor_;
	public:
		explicit market_data_receiver(
			const size_t trades_receive_threads_size,
			const size_t quotes_receive_threads_size );

	};
}

#endif // _MULTICAST_COMMUNICATION_MARKET_DATA_RECEIVER_H_
