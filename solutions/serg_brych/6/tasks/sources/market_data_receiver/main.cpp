#include <iostream>
#include <csignal>
#include <cstdint>
#include <boost/thread.hpp>
#include <fstream>

#include "market_data_receiver.h"
#include "data_processor.h"



boost::mutex close_mutex;
boost::condition_variable close_condition;


void signalHandler( int )
{
	close_condition.notify_one();
}

int main()
{
	std::cout << "maker_data_reciver starting" << std::endl;
	signal(SIGINT, signalHandler);  
	std::string config_file_path = BINARY_DIR "/config.ini";
	std::string result_file_path = BINARY_DIR "/market_data.dat";
	multicast_communication::config_reader* pcr = new multicast_communication::config_reader(config_file_path);
	
	std::ofstream result_file(result_file_path);

	multicast_communication::data_processor processor(result_file);
	
	multicast_communication::market_data_receiver receive(processor, pcr);

	std::cout << "maker_data_reciver started				OK" << std::endl;
	receive.run();
		
	boost::mutex::scoped_lock lock( close_mutex );
    close_condition.wait( lock );
	
	receive.stop();
	std::cout << "maker_data_reciver finished				OK" << std::endl;
	return 0;
}
