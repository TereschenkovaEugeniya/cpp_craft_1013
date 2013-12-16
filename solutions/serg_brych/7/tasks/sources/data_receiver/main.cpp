#include <iostream>
#include <csignal>
#include <cstdint>
#include <boost/thread.hpp>
#include <fstream>
#include "minute_market_data_processor.h"

boost::mutex close_mutex;
boost::condition_variable close_condition;

void signalHandler( int )
{
	close_condition.notify_one();
}

int main()
{
	minute_market_data::minute_market_data_processor processor;
	std::cout << "data_reciver starting" << std::endl;
	processor.run();

	signal(SIGINT, signalHandler);  
	boost::mutex::scoped_lock lock( close_mutex );
    close_condition.wait( lock );
    
	std::cout << "maker_data_reciver finished				OK" << std::endl;
	return 0;
}
