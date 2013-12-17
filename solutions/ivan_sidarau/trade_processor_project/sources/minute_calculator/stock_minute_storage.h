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

		const std::string filepath_;

		typedef std::map< std::string, ts_minute_storage_ptr > minutes;

		boost::mutex map_mutex_;
		minutes minutes_;
	public:
		explicit stock_minute_storage( const std::string& filepath );
		~stock_minute_storage();
	private:
		virtual void new_trade( multicast_communication::trade_message_ptr& );
		virtual void new_quote( multicast_communication::quote_message_ptr& );

		template< typename data_type >
		void new_data_( const data_type& data );

		void print_data_( const std::string& name, const stock_minute_data_ptr& data );
	};

	template< typename data_type >
	void stock_minute_storage::new_data_( const data_type& data )
	{
		const std::string& security_name = data->security_symbol();
		stock_minute_data_ptr minute_storage;
		{
			boost::mutex::scoped_lock lock( map_mutex_ );
			minutes::iterator i = minutes_.find( security_name );
			if ( i == minutes_.end() )
				i = minutes_.insert( std::make_pair( security_name, ts_minute_storage_ptr( new ts_minute_storage() ) ) ).first;
			minute_storage = i->second->new_data( data );
		}
		print_data_( security_name, minute_storage );
	}


}

#endif // _MINUTE_CALCULATOR_STOCK_MINUTE_STORAGE_H_
