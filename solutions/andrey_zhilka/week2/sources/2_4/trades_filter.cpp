#include "Message.h"
#include <iostream>
#include <fstream>

void filterMessages();

int main()
{	
	filterMessages();

	return 0;
}

void filterMessages() {
	unsigned maxT = 0;
	const char* input_file_path = SOURCE_DIR "/tests/2_4/2.4_example.in";
	const char* output_file_path = SOURCE_DIR "/tests/2_4/2.4_output.out";
	
	std::ifstream input( SOURCE_DIR "/tests/2_4/input24.txt", std::ios::binary );
	std::ofstream output( output_file_path, std::ios::binary );

	if ( !input.is_open() || !output.is_open() ) {
		std::cerr << "Failed to open input/output file\n";
		exit(1);
	}
	
	Message new_message;

	while ( true ) {
		input >> new_message;

		if ( input.eof() )
		{
			break;
		}

		if ( !new_message.isOk() )
		{
			continue;
		}

		if ( maxT < 2 || new_message.getTime() > maxT - 2 ) {
			output << new_message;
			maxT = ( maxT < new_message.getTime() ) ? new_message.getTime() : maxT;
		}
	}

	input.close();
	output.close();
}