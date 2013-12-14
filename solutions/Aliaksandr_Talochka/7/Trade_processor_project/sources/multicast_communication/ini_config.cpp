#include "ini_config.h"

#include <iostream>
#include <boost/regex.hpp>

multicast_communication::ini_config::ini_config(const std::string& iniFileName)
	: trade_thread_size(0), quote_thread_size(0)
{
	//std::string fileName = std::string(SOURCE_DIR) + "/sources/etc/configs.ini";
	std::ifstream *inputFile = new std::ifstream(iniFileName.c_str());

	if ( inputFile->is_open() )
	{
		trade_thread_size = read_integer(inputFile);
		quote_thread_size = read_integer(inputFile);

		trades_address_port = read_addresses(inputFile, read_integer(inputFile));
		quotes_address_port = read_addresses(inputFile, read_integer(inputFile));
	}
	delete inputFile;
}

multicast_communication::ini_config multicast_communication::ini_config::instance(const std::string& iniFileName)
{
	static ini_config ini(iniFileName);
	return ini;
}

multicast_communication::vector_address_port multicast_communication::ini_config::get_trades_address_port() const
{
	return trades_address_port;
}

multicast_communication::vector_address_port multicast_communication::ini_config::get_quotes_address_port() const
{
	return quotes_address_port;
}

size_t multicast_communication::ini_config::get_trade_thread_size() const
{
	return trade_thread_size;
}

size_t multicast_communication::ini_config::get_quote_thread_size() const
{
	return quote_thread_size;
}

size_t multicast_communication::ini_config::read_integer(std::ifstream *stream)
{
	std::string value;
	std::getline(*stream, value);
	return atoi(value.c_str());
}

multicast_communication::vector_address_port multicast_communication::ini_config::read_addresses(std::ifstream *stream, size_t count)
{
	multicast_communication::vector_address_port ret;
	std::string value;
	for (size_t i = 0; i < count; ++i)
	{
		std::getline(*stream, value);
		boost::smatch xResults;
		boost::regex xRegEx("((\\d{1,3}.){3}\\d{1,3})\\s(\\d+)");
		if (boost::regex_match(value, xResults, xRegEx))
		{
			std::string g = xResults[3];

			multicast_communication::pair_address_port address_port (xResults[1], atoi(std::string(xResults[3]).c_str()));
				
			ret.push_back(address_port);
		}
	}
	return ret;
}