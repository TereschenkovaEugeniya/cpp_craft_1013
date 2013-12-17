#include <iostream>
#include <fstream>
#include "palindrome.h"

using namespace std;

int main( int argc, char* argv[] )
{
	//open files for read and write
	ifstream input_file( BINARY_DIR "/input.txt" );
	if ( !input_file.is_open() )
		return -1;
	ofstream output_file( BINARY_DIR "/output.txt" );
	if ( !output_file.is_open() )
		return -1;

	string text, key;					//variables for text and keys
	getline( input_file, text );		//read text from file
	string_preparation(text);
	while(!input_file.eof())
	{
		getline( input_file, key );		//read keys from text
		if(key.empty())					//skipp emty line
			continue;
		if(find_key(key, text))			//comparison of text and keys and output result to file
			output_file << "YES" << endl;
		else
			output_file << "NO" << endl;
	}
	//close of files
	input_file.close();
	output_file.close();
	return 0;
}
