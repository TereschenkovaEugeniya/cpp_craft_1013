#include "market_data_receiver.h"


multicast_communication::market_data_receiver::market_data_receiver(const std::string& iniFileName)
{
	addr_port_TRADE = multicast_communication::ini_config::instance(iniFileName).get_trades_address_port();
	addr_port_QUOTE = multicast_communication::ini_config::instance(iniFileName).get_quotes_address_port();
	trade_thread_size = multicast_communication::ini_config::instance(iniFileName).get_trade_thread_size();
	quote_thread_size = multicast_communication::ini_config::instance(iniFileName).get_quote_thread_size();
}


multicast_communication::market_data_receiver::~market_data_receiver()
{
	stop();
}

void multicast_communication::market_data_receiver::start()
{
		
	for(vector_address_port::iterator it = addr_port_TRADE.begin(); it != addr_port_TRADE.end(); ++it)
	{
		boost::shared_ptr < multicast_communication::udp_listener > ptr_udp_listener (new udp_listener(io_service_trade, it->first, it->second, 'T'));
		TradesListeners.push_back(ptr_udp_listener);
	}

	for(vector_address_port::iterator it = addr_port_QUOTE.begin(); it != addr_port_QUOTE.end(); ++it)
	{
		boost::shared_ptr < multicast_communication::udp_listener > ptr_udp_listener (new udp_listener(io_service_quote, it->first, it->second, 'Q'));
		QuotesListeners.push_back(ptr_udp_listener);
	}



	for(size_t i = 0; i < trade_thread_size; i++)
	{
		thread_group_trade.create_thread(boost::bind(&market_data_receiver::run_service, this, boost::ref(io_service_trade)));
	}

	for(size_t i = 0; i < quote_thread_size; i++)
	{
		thread_group_quote.create_thread(boost::bind(&market_data_receiver::run_service, this, boost::ref(io_service_quote)));
	}
}


void multicast_communication::market_data_receiver::stop()
{
	io_service_trade.stop();
	io_service_quote.stop();

	thread_group_quote.join_all();
	thread_group_trade.join_all();

	TradesListeners.clear();
	QuotesListeners.clear();


}

std::vector<messages::trade_message> multicast_communication::market_data_receiver::get_trade_messages()
{
	std::vector<messages::trade_message> out;

	for(std::vector < boost::shared_ptr < multicast_communication::udp_listener > >::iterator it = TradesListeners.begin(); it != TradesListeners.end(); ++it)
	{
		multicast_communication::push_vector<messages::trade_message>(out, (*it)->get_trade_messages());
	}	

	return out;
}

std::vector<messages::quote_message> multicast_communication::market_data_receiver::get_quote_messages()
{
	std::vector<messages::quote_message> out;

	for(std::vector < boost::shared_ptr < multicast_communication::udp_listener > >::iterator it = QuotesListeners.begin(); it != QuotesListeners.end(); ++it)
	{
		multicast_communication::push_vector<messages::quote_message>(out, (*it)->get_quote_messages());
	}	
	return out;
}



void multicast_communication::market_data_receiver::run_service(boost::asio::io_service& io_serv)
{
	io_serv.run();
}
