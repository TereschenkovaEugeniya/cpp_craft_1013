#include "Message.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <map>

typedef std::map< unsigned, std::vector< unsigned > >::iterator map_iterator;
typedef std::map< unsigned, std::vector< unsigned > >::const_iterator const_map_iterator;

void reading_storingProcess( std::map< unsigned, std::vector< unsigned > >& );
void init_new_type( std::map< unsigned, std::vector< unsigned > >&, const Message& );
void update_report( std::map< unsigned, std::vector< unsigned > >&, const Message&, unsigned);
void flush_summary( const std::map< unsigned, std::vector< unsigned > >& );
void check_is_file_open( const std::fstream&, const char* ); 

int main()
{
	std::map< unsigned, std::vector< unsigned > > message_report;
	reading_storingProcess( message_report );
	flush_summary( message_report );
	return 0;
}

void reading_storingProcess( std::map< unsigned, std::vector< unsigned > >& message_report ) 
{
	const char *input_file_path = SOURCE_DIR "/tests/2_5/2.5_example.in";
	std::fstream input_file( input_file_path, std::ios::binary | std::ios::in );

	check_is_file_open( input_file, input_file_path );

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

		update_report( message_report, newMessage, new_message_capacity );
	}

	input_file.close();
}

void update_report( std::map< unsigned, std::vector< unsigned > >& message_report, 
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

void flush_summary( const std::map< unsigned, std::vector< unsigned > >& message_report)
{
	const char* output_file_path = SOURCE_DIR "/tests/2_5/2.5_output.out";
	std::fstream output_file( output_file_path, std::ios::binary | std::ios::out );
	
	check_is_file_open( output_file, output_file_path );
	
	for (const_map_iterator it = message_report.begin(); it != message_report.end(); it++ )
	{
		output_file.write( reinterpret_cast< const char* > ( &it->first ), sizeof( unsigned ) );
		double average = double ( it->second[2] ) / it->second[1];
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
void check_is_file_open( const std::fstream& file, const char *path)
{
	if ( !file.is_open() )
	{
		std::cerr << "Failed to open file " << path << std::endl;
		exit( 1 );
	}
}