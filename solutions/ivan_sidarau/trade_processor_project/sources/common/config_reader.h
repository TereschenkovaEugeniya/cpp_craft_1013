#ifndef _COMMON_CONFIG_READER_H_
#define _COMMON_CONFIG_READER_H_

#include <istream>

#include <address.h>

namespace common
{
	class config_reader
	{
		static const size_t max_receive_thread_size = 32ul;
		static const size_t max_listen_addresses_size = 10000ul;

		size_t trade_receive_threads_size_;
		size_t quote_receive_threads_size_;

		addresses trade_listen_addresses_;
		addresses quote_listen_addresses_;

	public:
		explicit config_reader( const std::string& );

	private:
		void check_thread_size_( const size_t readed_size );
		void read_addresses_( std::istream&, addresses& );
	public:
		size_t trade_receive_threads_size() const;
		size_t quote_receive_threads_size() const;

		const addresses& trade_listen_addresses() const;
		const addresses& quote_listen_addresses() const;
	};
}

#endif // _COMMON_CONFIG_READER_H_
