#ifndef _MULTICAST_COMMUNICATION_MEDIATOR_H_
#define _MULTICAST_COMMUNICATION_MEDIATOR_H_

#include <boost/noncopyable.hpp>

#include <config_reader.h>
#include <market_datafeed_writer.h>
#include <cts_parser.h>
#include <cqs_parser.h>
#include <market_data_receiver.h>

namespace multicast_communication
{
	class mediator : virtual protected boost::noncopyable,
		public market_data_receiver_manager
	{
		common::config_reader config_;
		market_datafeed_writer writer_;

		cts_parser cts_parser_;
		cqs_parser cqs_parser_;

		boost::shared_ptr< market_data_receiver > receiver_;
	public:
		explicit mediator( const std::string& filepath = "" );
		~mediator();
	private:
		void cts_line_message( const common::buffer_ptr ptr, const size_t size );
		void cqs_line_message( const common::buffer_ptr ptr, const size_t size );
	};
}

#endif // _MULTICAST_COMMUNICATION_MEDIATOR_H_
