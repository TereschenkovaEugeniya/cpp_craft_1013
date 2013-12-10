#ifndef _MULTICAST_COMMUNICATION_CQS_PARSER_H_
#define _MULTICAST_COMMUNICATION_CQS_PARSER_H_

#include <boost/noncopyable.hpp>

#include <buffer.h>

#include <quote_message.h>

namespace multicast_communication
{
	class cqs_parser;

	class quote_processor : virtual protected boost::noncopyable
	{
		friend class cqs_parser;

	protected:
		explicit quote_processor(){}
		virtual ~quote_processor(){}

		virtual void new_quote( quote_message_ptr& new_message ) = 0;
	};
	class cqs_parser : virtual protected boost::noncopyable
	{
	public:
		enum message_borders
		{
			SOH = 0x01,
			ETX = 0x03,
			US = 0x1F
		};
	private:
		quote_processor& quote_processor_;
	public:
		explicit cqs_parser( quote_processor& );
		~cqs_parser();

		void process_message( const common::buffer_ptr new_message, const size_t size );
	private:
		void filter_messages_( const char* parsing_ptr, size_t size );
		const char* parse_short_quote_( const char* parsing_ptr, size_t size );
		const char* parse_long_quote_( const char* parsing_ptr, size_t size );
	};
}

#endif // _MULTICAST_COMMUNICATION_CQS_PARSER_H_

