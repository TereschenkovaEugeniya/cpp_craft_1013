#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>

void clearString(std::string& line);

int main()
{
    static const std::string yes = "YES";
    static const std::string no = "NO";

    //open
    std::ifstream inFile	( BINARY_DIR "input.txt");
    std::ofstream outFile	( BINARY_DIR "output.txt");
	if (!inFile.is_open()) {
		return 1;
	}

    //init mainLine
    std::string mainLine;
    std::getline(inFile, mainLine);
    clearString(mainLine);

    // string comparison
    std::string testLine;
    bool eqLines = false;
    while ( ! inFile.eof() ) {
        std::getline( inFile, testLine );
        if ( ! testLine.empty() ) {
            clearString(testLine);
            eqLines = std::search( mainLine.rbegin(), mainLine.rend(), testLine.begin(), testLine.end() ) != mainLine.rend();
            outFile  << ( eqLines ? yes : no ) << std::endl;
        }
    }

    //close
    outFile.close();
    inFile.close();
}

//Does not convert the Cyrillic alphabet
void clearString(std::string& line)
{
	static const char space = ' ';
	static const char dash = '-';
	static const char slash = '\\';
    std::replace(line.begin(), line.end(), dash, space);
    std::replace(line.begin(), line.end(), slash, space);
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);
    line.erase(std::remove(line.begin(), line.end(), space), line.end());
}
