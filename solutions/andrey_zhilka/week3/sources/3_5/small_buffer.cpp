#include "Message.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <map>

#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

typedef std::map< unsigned, std::vector< unsigned > >::iterator map_iterator;
typedef std::map< unsigned, std::vector< unsigned > >::const_iterator const_map_iterator;

bool reading_storingProcess( std::map< unsigned, std::vector< unsigned > >&, std::string );
void init_new_type( std::map< unsigned, std::vector< unsigned > >&, const Message& );
void update_file_report( std::map< unsigned, std::vector< unsigned > >&, const Message&, unsigned);
void flush_summary();
void update_general_report( const std::map< unsigned, std::vector< unsigned > >& );
bool check_is_file_open( const std::fstream&, const char* );
void thread_main( std::string );

std::map< unsigned, std::vector< unsigned > > general_message_report;
static boost::mutex report_mutex;

static const int max_file_number = 999;

int main()
{
	boost::thread_group file_processors;
	std::stringstream strim;

	for ( unsigned i = 0; i < max_file_number; i++ )
	{
		strim.width( 3 );
		strim << std::setfill( '0' );
		strim << i;
		
		std::string number;
		strim >> number;

		std::string input_file_path = SOURCE_DIR "/tests/3_5/input_";
		input_file_path += number + ".txt";

		boost::filesystem2::path next_file(input_file_path.c_str());
		
		if ( boost::filesystem2::exists( next_file ) )
		{
			file_processors.add_thread( new boost::thread( &thread_main, input_file_path ) );
		}
		
		strim.clear();
	}

	file_processors.join_all();

	flush_summary();
	return 0;
}

void thread_main( std::string file_path )
{
	std::map< unsigned, std::vector< unsigned > > thread_message_report;
	if ( !reading_storingProcess( thread_message_report, file_path ) )
	{
		return;
	}
	update_general_report( thread_message_report );
}

bool reading_storingProcess( std::map< unsigned, std::vector< unsigned > >& message_report,
							std::string file_path) 
{
	std::fstream input_file( file_path.c_str(), std::ios::binary | std::ios::in );

	if ( !check_is_file_open( input_file, file_path.c_str() ) )
	{
		return false;
	}

	unsigned new_message_capacity;
	Message newMessage;

	while ( true ) 
	{
		input_file >> newMessage;

		if ( input_file.eof() )
		{
			break;
		}

		if ( newMessage.getType() > 100000 )
		{
			continue;
		}

		new_message_capacity = 3 * sizeof( unsigned ) + newMessage.getLength();

		update_file_report( message_report, newMessage, new_message_capacity );
	}

	input_file.close();

	return true;
}

void update_file_report( std::map< unsigned, std::vector< unsigned > >& message_report, 
				   const Message& message, unsigned new_message_capacity )
{
		map_iterator existing_statistics = message_report.find( message.getType() );
		
		if ( existing_statistics == message_report.end() )
		{
			init_new_type( message_report, message);
			existing_statistics = message_report.find( message.getType() );
		}
		
		if ( existing_statistics->second[1] == message.getTime()
			&& existing_statistics->second[0] < new_message_capacity ) 
		{
			return;
		}
		
		if ( existing_statistics->second[1] != message.getTime() )
		{
			existing_statistics->second[0] = 2048;
			existing_statistics->second[1] = message.getTime();
		}

		existing_statistics->second[0] -= new_message_capacity;
		existing_statistics->second[2]++;
}

void update_general_report( const std::map< unsigned, std::vector< unsigned > >& file_report )
{
	boost::mutex::scoped_lock lock( report_mutex );

	for ( const_map_iterator it = file_report.begin(); it != file_report.end(); it++ )
	{
		if ( general_message_report.find( it->first ) == general_message_report.end() )
		{
			std::vector< unsigned > new_type_start;
			new_type_start.push_back( 0 ); // total time
			new_type_start.push_back( 0 ); // total number

			general_message_report[ it->first ] = new_type_start;
		}

		general_message_report[ it->first ][1] += it->second[2];
		
		if ( general_message_report[ it->first ][0] < it->second[1] )
		{
			general_message_report[ it->first ][0] = it->second[1];
		}
	}
}

void flush_summary()
{
	const char* output_file_path = SOURCE_DIR "/tests/3_5/output.txt";
	std::fstream output_file( output_file_path, std::ios::binary | std::ios::out );
	
	check_is_file_open( output_file, output_file_path );
	
	for (const_map_iterator it = general_message_report.begin(); it != general_message_report.end(); it++ )
	{
		output_file.write( reinterpret_cast< const char* > ( &it->first ), sizeof( unsigned ) );
		double average = double ( it->second[1] ) / it->second[0];
		output_file.write( reinterpret_cast< const char* > ( &average ), sizeof( double ) );
	}

	output_file.close();
}

void init_new_type( std::map< unsigned, std::vector< unsigned > >& message_report,
				   const Message& message)
{
	std::vector< unsigned > message_type_statistics;
	message_type_statistics.push_back( 2048 ); // free space
	message_type_statistics.push_back( message.getTime() );
	message_type_statistics.push_back( 0 );	// total number

	message_report[ message.getType() ] = message_type_statistics;
}
bool check_is_file_open( const std::fstream& file, const char *path)
{
	if ( !file.is_open() )
	{
		std::cerr << "Failed to open file " << path << std::endl;
		return false;
	}

	return true;
}