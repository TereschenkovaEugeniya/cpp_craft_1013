#ifndef _MULTICAST_COMMUNICATION_MARKET_DATAFEED_WRITER_H_
#define _MULTICAST_COMMUNICATION_MARKET_DATAFEED_WRITER_H_

#include <string>

#include <file_logger.h>
#include <queue_logger.h>
#include <task_processor.h>

#include <market_data_processor.h>

namespace multicast_communication
{
	namespace tests_
	{
		namespace detail
		{
			class market_datafeed_writer_test_helper;
		}
	}

	class market_datafeed_writer : public market_data_processor
	{
		friend class tests_::detail::market_datafeed_writer_test_helper;

		static const size_t writing_threads_size = 1ul;
		typedef system_utilities::common::queue_logger< true, false, false > queue_logger;

		queue_logger::tasker log_writer_;
		system_utilities::common::file_logger< queue_logger > output_;
	public:
		explicit market_datafeed_writer( const std::string& filename );
		virtual ~market_datafeed_writer();
	private:
		virtual void new_trade( const trade_message_ptr& );
		virtual void new_quote( const quote_message_ptr& );
	};
}

#endif // _MULTICAST_COMMUNICATION_MARKET_DATAFEED_WRITER_H_
