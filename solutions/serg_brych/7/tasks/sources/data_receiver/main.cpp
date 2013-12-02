#include <iostream>
#include <csignal>
#include <cstdint>
#include <boost/thread.hpp>
#include <fstream>

boost::mutex close_mutex;
boost::condition_variable close_condition;


void signalHandler( int )
{
	close_condition.notify_one();
}

int main()
{
	std::cout << "data_reciver starting" << std::endl;
	signal(SIGINT, signalHandler);  

	std::string config_file_path = BINARY_DIR "/config.ini";
	std::string result_file_path = BINARY_DIR "/market_data.dat";

	boost::mutex::scoped_lock lock( close_mutex );
    	close_condition.wait( lock );
        
	std::cout << "maker_data_reciver finished				OK" << std::endl;
	return 0;
}
