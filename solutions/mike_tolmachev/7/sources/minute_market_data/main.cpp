#include "minute_market_data.h"
#include <csignal>

boost::condition_variable cond;

void signal_handler(int)
{
	cond.notify_all();
}

int main()
{
    using namespace multicast_communication;
    
	const std::string config_path(SOURCE_DIR "/tests/data/config.ini");
	const std::string output_dir(BINARY_DIR "/market_data.dat");

	minute_calculator::minute_market_data mmd(config_path, output_dir);

	boost::mutex mtx;

	std::signal(SIGINT, signal_handler);
	boost::mutex::scoped_lock lock(mtx);
	cond.wait(lock);
}
