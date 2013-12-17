#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>

bool comparing_function(double value, double code);

int main()
{
    std::ifstream input_file	( SOURCE_DIR "/tests/data/002/input.txt" );
    std::ofstream output_file	( SOURCE_DIR "/tests/data/002/output.txt");

    int N;
    input_file >> N;

    std::vector<double> codes;
    codes.reserve(N);

    double value;
    for (int i=0; i<N; i++) {
        input_file >> value;
        codes.push_back(value);
    }

    std::sort(codes.begin(),codes.end());
    while ( true ) {
        input_file >> value;
        if ( input_file.eof() ) {
            break;
        }
        output_file << ( std::binary_search(codes.begin(), codes.end(), value, comparing_function ) ? "YES" : "NO" ) << std::endl;
    }
    output_file.close();
    input_file.close();
}

bool comparing_function(double code, double value)
{
    return ( int ) ( code * 10000 ) < ( int ) ( value * 10000 );
}

