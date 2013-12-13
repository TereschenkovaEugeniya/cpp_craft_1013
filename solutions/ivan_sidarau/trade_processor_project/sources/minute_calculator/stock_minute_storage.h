#ifndef _MINUTE_CALCULATOR_STOCK_MINUTE_STORAGE_H_
#define _MINUTE_CALCULATOR_STOCK_MINUTE_STORAGE_H_

#include <map>
#include <fstream>

#include <boost/noncopyable.hpp>

#include <cqs_parser.h>
#include <cts_parser.h>

#include <stock_minute_data.h>
#include <ts_minute_storage.h>

namespace minute_calculator
{
	namespace tests_
	{
		namespace detail
		{
			class stock_minute_storage_test_helper;
		}
	}

	class stock_minute_storage : 
		public multicast_communication::trade_processor, 
		public multicast_communication::quote_processor
	{
		friend class tests_::detail::stock_minute_storage_test_helper;

		std::ofstream output_;

		typedef std::map< std::string, ts_minute_storage_ptr > minutes;

		boost::mutex map_mutex_;
		minutes minutes_;
	public:
		explicit stock_minute_storage( const std::string& filename );
		~stock_minute_storage();
	private:
		virtual void new_trade( multicast_communication::trade_message_ptr& );
		virtual void new_quote( multicast_communication::quote_message_ptr& );

		template< typename data_type >
		void new_data_( const data_type& data );
	};

	template< typename data_type >
	void stock_minute_storage::new_data_( const data_type& data )
	{
		const std::string& security_name = data->security_symbol();
		ts_minute_storage_ptr minute_storage;
		{
			boost::mutex::scoped_lock lock( map_mutex_ );
			minute_storage = minutes_[ security_name ];
			if ( !minute_storage )
				minute_storage.reset( new ts_minute_storage() );
		}
		stock_minute_data_ptr minute_to_store = minute_storage->new_data( data );
		if ( minute_to_store )
			minute_to_store->print_binary( output_ );
	}


}

#endif // _MINUTE_CALCULATOR_STOCK_MINUTE_STORAGE_H_
