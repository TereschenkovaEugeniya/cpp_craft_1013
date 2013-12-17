#include <iostream>
#include <cctype>
#include <string>
#include <fstream>
#include <algorithm>

void adaptString( std::string&, bool );
void inverse( std::string& );
bool is_useless( char );

int main()
{
	std::ifstream input( SOURCE_DIR "/sources/task1_3/input.txt");
	std::ofstream output( SOURCE_DIR "/tests/task1_3/output.txt");

	if ( !input.is_open() )
	{
		std::cerr << "Failed to open input file\n";
		return 1;
	}
	if ( !output.is_open() )
	{
		std::cerr << "Failed to create output file\n";
		return 1;
	}

	std::string text;
	std::string key;

	bool toInvert = false;
	std::getline(input, text);

	adaptString(text, toInvert);

	toInvert = true;
	while( std::getline( input, key ) )
	{
		adaptString( key, toInvert );
		if ( text.find( key ) != std::string::npos)
		{
			output << "YES\n";
		}
		else
		{
			output << "NO\n";
		}
	}

	input.close();
	output.close();
	return 0;
}

void adaptString( std::string& target, bool toInverse )
{
	target.erase( std::remove_if(target.begin(), target.end(), is_useless), target.end() );
	std::transform( target.begin(), target.end(), target.begin(), std::tolower );	

	if (toInverse)
	{
		reverse( target.begin(), target.end() );
	}
}

bool is_useless( char c )
{
	return ( c == ' ' || c == '\\' || c == '-' );
}