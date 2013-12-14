#ifndef __INI_CONFIG_H__
#define __INI_CONFIG_H__

#include <iostream>
#include <fstream>

#include <vector>
#include <string>

namespace multicast_communication
{
	typedef std::pair < std::string, unsigned short > pair_address_port;
	typedef std::vector < pair_address_port > vector_address_port;

	class ini_config
	{
	public:
		static multicast_communication::ini_config instance(const std::string& iniFileName = std::string(BINARY_DIR"/configs.ini"));

		vector_address_port get_trades_address_port() const;
		vector_address_port get_quotes_address_port() const;
		size_t get_trade_thread_size() const;
		size_t get_quote_thread_size() const;

	private:
		ini_config(const std::string& iniFileName);
		ini_config& operator=(const ini_config&);

		size_t read_integer(std::ifstream *stream);
		vector_address_port read_addresses(std::ifstream *stream, size_t count);

		size_t trade_thread_size;
		size_t quote_thread_size;

		vector_address_port trades_address_port;
		vector_address_port quotes_address_port;
	};
}

#endif //__INI_CONFIG_H__