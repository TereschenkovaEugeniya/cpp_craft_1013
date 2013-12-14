#include "market_data_sender.h"


multicast_communication::market_data_sender::market_data_sender(const std::string& iniFileName, const std::string& PathToData)
	: Path(PathToData)
{
	addr_port_TRADE = multicast_communication::ini_config::instance(iniFileName).get_trades_address_port();
	addr_port_QUOTE = multicast_communication::ini_config::instance(iniFileName).get_quotes_address_port();
	trade_thread_size = multicast_communication::ini_config::instance(iniFileName).get_trade_thread_size();
	quote_thread_size = multicast_communication::ini_config::instance(iniFileName).get_quote_thread_size();
}


multicast_communication::market_data_sender::~market_data_sender()
{
	stop();
}

void multicast_communication::market_data_sender::start()
{
		
	for(vector_address_port::iterator it = addr_port_TRADE.begin(); it != addr_port_TRADE.end(); ++it)
	{
		std::string FileName = Path + '/' + it->first + ".udp";
		boost::shared_ptr < multicast_communication::sender > ptr_udp_sender (new sender(io_service_trade, it->first, it->second, FileName, SampleTime));
		TradesSenders.push_back(ptr_udp_sender);
	}

	for(vector_address_port::iterator it = addr_port_QUOTE.begin(); it != addr_port_QUOTE.end(); ++it)
	{
		std::string FileName = Path + '/' + it->first + ".udp";
		boost::shared_ptr < multicast_communication::sender > ptr_udp_sender (new sender(io_service_quote, it->first, it->second, FileName, SampleTime));
		QuotesSenders.push_back(ptr_udp_sender);
	}

	for(size_t i = 0; i < trade_thread_size; i++)
	{
		thread_group_trade.create_thread(boost::bind(&market_data_sender::run_service, this, boost::ref(io_service_trade)));
	}

	for(size_t i = 0; i < quote_thread_size; i++)
	{
		thread_group_quote.create_thread(boost::bind(&market_data_sender::run_service, this, boost::ref(io_service_quote)));
	}
}


void multicast_communication::market_data_sender::stop()
{
	io_service_trade.stop();
	io_service_quote.stop();

	thread_group_quote.join_all();
	thread_group_trade.join_all();

	TradesSenders.clear();
	QuotesSenders.clear();
}


void multicast_communication::market_data_sender::run_service(boost::asio::io_service& io_serv)
{
	io_serv.run();
}
