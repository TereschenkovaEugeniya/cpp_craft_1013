#include <config_reader.h>

#include <mediator.h>

boost::mutex wait_ctrl_c_protector;
boost::condition_variable pressed;

void ctrl_c_handler( int )
{
	boost::mutex::scoped_lock lock( wait_ctrl_c_protector );
	pressed.notify_one();
}

int main()
{
	try
	{
		minute_calculator::mediator m( "." );

		boost::mutex::scoped_lock lock( wait_ctrl_c_protector );
		signal(SIGINT, ctrl_c_handler);  

		pressed.wait( lock );
	}
	catch( const std::exception& ex )
	{
		std::cerr << "Exception: " << ex.what() << std::endl;
	}
	catch( ... )
	{
		std::cerr << "Unknown exception throwed" << std::endl;
	}
	std::cout << "Finished" << std::endl;
}
