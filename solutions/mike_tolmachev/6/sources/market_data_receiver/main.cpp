#include "market_data_receiver.h"
#include "market_data_processor_helper.h"
#include <boost/thread.hpp>
#include <csignal>

boost::condition_variable cond;

void signal_handler(int)
{
	cond.notify_all();
}

int main()
{
    using namespace multicast_communication;
    
	std::string config_path(SOURCE_DIR "/tests/data/config.ini");
	std::ofstream processor_output(BINARY_DIR "/market_data.dat");

	market_data_processor_helper processor(processor_output);
	market_data_receiver receiver(config_path, processor);

	boost::mutex mtx;

	std::signal(SIGINT, signal_handler);
	boost::mutex::scoped_lock lock(mtx);
	cond.wait(lock);
}
