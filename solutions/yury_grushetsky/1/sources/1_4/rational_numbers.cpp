#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

static const long double eps = 1E-4;
bool mycomp(long double v1, long double v2)
{
	return v2-v1>eps;
}

int main( int argc, char* argv[] )
{
	ifstream input_file( BINARY_DIR "/input.txt" );
	ofstream output_file( BINARY_DIR "/output.txt");
	if ( !input_file.is_open())
	{
	    cerr << "input file does not exist" << endl;
	    return 1;
	}
	long int N; // N < 1 000 000
	input_file >> N;
	vector<long double> codes (N);
	for(vector<long double>::iterator it = codes.begin(); it!=codes.end(); ++it)
		input_file >> *it;
	sort(codes.begin(), codes.end());
	long double curval;
	while( input_file >> curval )
	{
		if( binary_search(codes.begin(), codes.end(), curval, mycomp))
			output_file << "YES" << endl;
		else
			output_file << "NO" << endl;
	}
	input_file.close();
	output_file.close();
}

