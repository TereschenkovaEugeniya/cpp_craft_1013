#include <processors_test_helper.h>


void multicast_communication::tests_::market_data_receiver_manager_test_helper::cts_line_message( 
	const common::buffer_ptr , const size_t size )
{
	boost::mutex::scoped_lock lock( cts_message_sizes_protector_ );
	cts_message_sizes_.push_back( size );
}
void multicast_communication::tests_::market_data_receiver_manager_test_helper::cqs_line_message( 
	const common::buffer_ptr , const size_t size )
{
	boost::mutex::scoped_lock lock( cqs_message_sizes_protector_ );
	cqs_message_sizes_.push_back( size );
}

std::pair< common::buffer_ptr, size_t > multicast_communication::tests_::detail::read_file( const std::string& filename )
{
	const size_t file_size = boost::filesystem::file_size( filename );
	std::ifstream r( filename.c_str(), std::ios::binary );
	std::pair< common::buffer_ptr, size_t > answer;
	answer.first.reset( new common::buffer() );
	answer.second = file_size;
	r.read( answer.first->data, file_size );
	return answer;
}
