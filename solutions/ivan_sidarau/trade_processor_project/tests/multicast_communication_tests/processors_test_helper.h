#ifndef _MARKET_DATA_PROCESSOR_TEST_HELPER_H_
#define _MARKET_DATA_PROCESSOR_TEST_HELPER_H_

#include <boost/filesystem.hpp>
#include <boost/shared_array.hpp>
	
#include <market_data_processor.h>
#include <market_data_receiver.h>

#include <cts_parser.h>
#include <cqs_parser.h>

namespace multicast_communication
{
	namespace tests_
	{
		namespace detail
		{
			template< typename cta_functor >
			size_t send_cts_file( const std::string& file_name, const cta_functor& );
			template< typename cta_functor >
			size_t send_cqs_file( const std::string& file_name, const cta_functor& );
			template< typename cta_functor >
			size_t send_file_with_delimeter( const std::string& file_name, const char delimeter, const cta_functor& );


			template< typename parser_type >
			class send_to_processor
			{
				parser_type& parser_;
			public:
				explicit send_to_processor( parser_type& parser )
					: parser_( parser )
				{
				}
				void operator()( const char* const buff, const size_t size ) const
				{
					common::buffer_ptr ptr( new common::buffer() );
					memcpy( ptr->data, buff, size );

					parser_.process_message( ptr, size );
				}
			};


		}
	}
}
template< typename cta_functor >
size_t multicast_communication::tests_::detail::send_cts_file( const std::string& file_name, const cta_functor& f )
{
	return send_file_with_delimeter( file_name, cts_parser::ETX, f );
}
template< typename cta_functor >
size_t multicast_communication::tests_::detail::send_cqs_file( const std::string& file_name, const cta_functor& f )
{
	return send_file_with_delimeter( file_name, cqs_parser::ETX, f );
}
template< typename cta_functor >
size_t multicast_communication::tests_::detail::send_file_with_delimeter( const std::string& file_name, const char delimeter, const cta_functor& f )
{
	const std::string file_path = (SOURCE_DIR "/tests/data/udp_data/" + file_name);

	const size_t file_size = boost::filesystem::file_size( file_path );
	boost::shared_array< char > buffer( new char[ file_size ] );

	std::ifstream ifs( file_path.c_str(), std::ios::binary );
	ifs.read( buffer.get(), file_size );
	ifs.close();

	size_t sended_messages = 0;
	char* i_begin = buffer.get();
	char* end_of_buffer = buffer.get() + file_size;
	char* i_end = std::find( i_begin, end_of_buffer, delimeter );

	while ( i_begin != end_of_buffer )
	{
		++sended_messages;

		f( i_begin, i_end - i_begin + 1 );

		i_begin = i_end + 1;
		i_end = std::find( i_begin, end_of_buffer, delimeter );
	}
	return sended_messages;
}

namespace multicast_communication
{
	namespace tests_
	{
		class market_data_receiver_manager_test_helper : public market_data_receiver_manager
		{
			boost::mutex cts_message_sizes_protector_;
			boost::mutex cqs_message_sizes_protector_;
		public:
			std::vector< size_t > cts_message_sizes_;
			std::vector< size_t > cqs_message_sizes_;

			explicit market_data_receiver_manager_test_helper(){}
			virtual ~market_data_receiver_manager_test_helper(){}
		private:
			virtual void cts_line_message( const common::buffer_ptr ptr, const size_t size );
			virtual void cqs_line_message( const common::buffer_ptr ptr, const size_t size );
		};

		class market_data_processor_test_helper : public market_data_processor
		{
		public:
			explicit market_data_processor_test_helper(){}
			virtual ~market_data_processor_test_helper(){}
		private:
			virtual void new_trade( const trade_message_ptr& );
			virtual void new_quote( const quote_message_ptr& );
		};
	}
}


#endif // _MARKET_DATA_PROCESSOR_TEST_HELPER_H_
