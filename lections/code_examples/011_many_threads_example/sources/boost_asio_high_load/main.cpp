#include <boost/thread.hpp>
#include <boost/asio.hpp>

void worker( boost::asio::io_service& service )
{
	service.run();
}

int main()
{
	boost::thread_group thread_group;
	boost::asio::io_service service;

	boost::shared_ptr< boost::asio::io_service::work > dummy_work( new boost::asio::io_service::work( service ) );

	for( size_t i = 0 ; i < 4 ; ++i )
		thread_group.create_thread( boost::bind( &worker, boost::ref( service ) ) );

	boost::this_thread::sleep_for( boost::chrono::milliseconds( 1500 ) );

	thread_group.create_thread( boost::bind( &worker, boost::ref( service ) ) );
	thread_group.create_thread( boost::bind( &worker, boost::ref( service ) ) );

	boost::this_thread::sleep_for( boost::chrono::milliseconds( 1500 ) );

	dummy_work.reset();

	boost::this_thread::sleep_for( boost::chrono::milliseconds( 1500 ) );

	thread_group.join_all();
}
