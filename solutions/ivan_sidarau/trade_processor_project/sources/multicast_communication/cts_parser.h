#ifndef _MULTICAST_COMMUNICATION_CTS_PARSER_H_
#define _MULTICAST_COMMUNICATION_CTS_PARSER_H_

#include <boost/noncopyable.hpp>

#include <buffer.h>

#include <trade_message.h>

namespace multicast_communication
{
	class trade_processor : virtual protected boost::noncopyable
	{
		friend class cts_parser;

	protected:
		explicit trade_processor(){}
		virtual ~trade_processor(){}
	private:
		virtual void new_trade( trade_message_ptr& new_message ) = 0;
	};

	class cts_parser : virtual protected boost::noncopyable
	{
	public:
		enum message_borders
		{
			SOH = 0x01,
			ETX = 0x03,
			US = 0x1F
		};
	private:
		trade_processor& trade_processor_;
	public:
		explicit cts_parser( trade_processor& processor );
		~cts_parser();

		void process_message( const common::buffer_ptr new_message, const size_t size );
	private:
		void filter_messages_( const char* parsing_ptr, size_t size );
		const char* parse_short_trade_( const char* parsing_ptr, size_t size );
		const char* parse_long_trade_( const char* parsing_ptr, size_t size );
	};
}

#endif // _MULTICAST_COMMUNICATION_CTS_PARSER_H_

