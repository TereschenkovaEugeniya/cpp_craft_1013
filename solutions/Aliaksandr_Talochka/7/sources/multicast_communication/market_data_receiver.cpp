#include "market_data_receiver.h"


multicast_communication::market_data_receiver::market_data_receiver(const std::string& iniFileName)
{
	addr_port_TRADE = multicast_communication::ini_config::instance(iniFileName)->get_trades_address_port();
	addr_port_QUOTE = multicast_communication::ini_config::instance(iniFileName)->get_trades_address_port();
	trade_thread_size = multicast_communication::ini_config::instance(iniFileName)->get_trade_thread_size();
	quote_thread_size = multicast_communication::ini_config::instance(iniFileName)->get_quote_thread_size();
}


multicast_communication::market_data_receiver::~market_data_receiver()
{
}

void multicast_communication::market_data_receiver::start()
{

}
