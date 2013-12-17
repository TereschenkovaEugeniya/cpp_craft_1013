#ifndef __CONFIG_READER_H__
#define __CONFIG_READER_H__

#include <vector>
#include <string>
#include <boost/noncopyable.hpp>
namespace multicast_communication
{
	typedef std::pair<std::string, unsigned short> address_and_port;
	typedef std::vector< address_and_port > addresses_and_ports;


	class config_reader : virtual protected boost::noncopyable
	{
	public:
		explicit config_reader(const std::string &);
		explicit config_reader(size_t,  size_t, addresses_and_ports &, addresses_and_ports &);
		size_t get_trade_thread_size() const;
		size_t get_quote_thread_size() const;
		const addresses_and_ports& get_trade_adr_and_ports() const;
		const addresses_and_ports& get_quote_adr_and_ports() const;
	private:
		size_t trade_thread_size_;
		size_t quote_thread_size_;
		addresses_and_ports trade_adr_ports_;
		addresses_and_ports quote_adr_ports_;

	};
}
#endif //__CONFIG_READER_H__