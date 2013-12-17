#include "islands.h"

int main( int argc, char* argv[] )
{
	//open files for read and write
	ifstream input_file( BINARY_DIR "/input.txt" );
	ofstream output_file( BINARY_DIR "/output.txt" );
	int islands, count_land;
	vector<char>* land;							// vectors for read of file
	land = read_file(input_file, count_land);	// read information from file to vectors
	islands = island(land, count_land);			// count the number of 'islands'
	output_file << islands;						// write result to file
	delete [] land;
	//close of files
	input_file.close();
	output_file.close();
	return 0;
}
