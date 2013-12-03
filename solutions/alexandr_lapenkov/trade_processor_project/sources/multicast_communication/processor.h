#ifndef _MULTICAST_COMMUNICATION_PROCESSOR_H_
#define _MULTICAST_COMMUNICATION_PROCESSOR_H_

#include "market_data_processor.h"
#include "quote_message.h"
#include "trade_message.h"
#include "binreader.h"
#include "thread_safe_queue.h"

#include <fstream>
#include <string>
#include <sstream>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>

namespace multicast_communication
{
	enum message_type { QUOTE, TRADE };
	
	class processor : public market_data_processor, private virtual boost::noncopyable
	{
	private:
		boost::mutex new_message_protector;
		std::ofstream out;
		static const char SOH;
		static const char EOT;
		static const char US;
	
	public:
		explicit processor();
		explicit processor( std::string filename );
		virtual ~processor();
		void parse( std::string& in, message_type mt );
		virtual void new_trade( const trade_message_ptr& );
		virtual void new_quote( const quote_message_ptr& );
	
	};
		
	typedef boost::shared_ptr< processor > processor_ptr;

}

#endif // _MULTICAST_COMMUNICATION_PROCESSOR_H_
