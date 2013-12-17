#include <fstream>
#include <iostream>

int main()
{
	std::ifstream in( BINARY_DIR "/output.txt", std::ios_base::binary );
	if ( !in.is_open() )
		return 1;
	while ( !in.eof() )
	{
		unsigned int c = 0;
		double d = 0;
		in.read( (char*)(&c), sizeof( c ) );
		if ( in.eof() )
			return 0;
		in.read( (char*)(&d), sizeof( d ) );
		std::cout << c << " " << d << std::endl;
	}
}

