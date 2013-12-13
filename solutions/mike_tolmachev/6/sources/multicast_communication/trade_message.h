#ifndef _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_
#define _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>
#include <map>

namespace multicast_communication
{
	class trade_message
	{
		std::map<char, double> denominators_;
		std::map<char, uint32_t> time_stamp_;

		std::string security_symbol_;
		double price_;
		double volume_;

		uint32_t time_;
	public:
		trade_message();

		std::string security_symbol() const;
		double price() const;
		double volume() const;

		uint32_t time() const;

		bool initialize(const std::string& msg);
	private:
		bool init_short(const std::string& body);
		bool init_long(const std::string& body);
	};

	typedef boost::shared_ptr< trade_message > trade_message_ptr;
}

#endif // _MULTICAST_COMMUNICATION_TRADE_MESSAGE_H_
