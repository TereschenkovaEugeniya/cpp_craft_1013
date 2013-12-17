#ifndef _MINUTE_CALCULATOR_MEDIATOR_H_
#define _MINUTE_CALCULATOR_MEDIATOR_H_

#include <boost/noncopyable.hpp>

#include <config_reader.h>
#include <stock_minute_storage.h>
#include <cts_parser.h>
#include <cqs_parser.h>
#include <market_data_receiver.h>

namespace minute_calculator
{
	class mediator : virtual protected boost::noncopyable,
		public multicast_communication::market_data_receiver_manager
	{
		common::config_reader config_;
		stock_minute_storage stock_minute_storage_;

		multicast_communication::cts_parser cts_parser_;
		multicast_communication::cqs_parser cqs_parser_;

		boost::shared_ptr< multicast_communication::market_data_receiver > receiver_;
	public:
		explicit mediator( const std::string& filepath );
		~mediator();
	private:
		void cts_line_message( const common::buffer_ptr ptr, const size_t size );
		void cqs_line_message( const common::buffer_ptr ptr, const size_t size );
	};
}

#endif // _MINUTE_CALCULATOR_MEDIATOR_H_
