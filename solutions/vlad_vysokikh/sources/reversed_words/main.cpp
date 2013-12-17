#include <fstream>
#include <string>
#include <algorithm>

void optimize_string(std::string& line)
{
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);
    std::replace(line.begin(), line.end(), '\\', ' ');
    std::replace(line.begin(), line.end(), '/', ' ');
    std::replace(line.begin(), line.end(), '-', ' ');
    line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
}

int main( int argc, char* argv[] )
{
    std::ifstream input_file	( SOURCE_DIR "/tests/data/001/input.txt" );
    std::ofstream output_file	( SOURCE_DIR "/tests/data/001/output.txt");

    std::string line;
    std::getline( input_file, line );

    optimize_string(line);

    std::string test_line;
    while ( ! input_file.eof() ) {
        std::getline( input_file, test_line );
        if ( ! test_line.empty() ) {	  
            optimize_string(test_line);
            std::string::const_reverse_iterator result = std::search( line.rbegin(), line.rend(), test_line.begin(), test_line.end() );
            output_file << ( (result != line.rend() ) ? "YES" : "NO" ) << std::endl;
        }
    }
    output_file.close();
    input_file.close();
}
