#ifndef _MARKET_DATA_PROCESSOR_TEST_HELPER_H_
#define _MARKET_DATA_PROCESSOR_TEST_HELPER_H_

#include <market_data_processor.h>
#include <market_data_receiver.h>

#include <cts_parser.h>
#include <cqs_parser.h>

namespace multicast_communication
{
	namespace tests_
	{
		class market_data_receiver_manager_test_helper : public market_data_receiver_manager
		{
			boost::mutex cts_message_sizes_protector_;
			boost::mutex cqs_message_sizes_protector_;
		public:
			std::vector< size_t > cts_message_sizes_;
			std::vector< size_t > cqs_message_sizes_;

			explicit market_data_receiver_manager_test_helper(){}
			virtual ~market_data_receiver_manager_test_helper(){}
		private:
			virtual void cts_line_message( const common::buffer_ptr ptr, const size_t size );
			virtual void cqs_line_message( const common::buffer_ptr ptr, const size_t size );
		};

		class market_data_processor_test_helper : public market_data_processor
		{
		public:
			explicit market_data_processor_test_helper(){}
			virtual ~market_data_processor_test_helper(){}
		private:
			virtual void new_trade( const trade_message_ptr& );
			virtual void new_quote( const quote_message_ptr& );
		};
	}
}


#endif // _MARKET_DATA_PROCESSOR_TEST_HELPER_H_
