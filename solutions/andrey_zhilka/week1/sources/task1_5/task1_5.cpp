#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

int calculateIslands( const std::vector< std::string >& );
void exploreIsland( unsigned, unsigned, const std::vector< std::string >&, bool** );

int main() 
{	
	std::vector<std::string> map;
	std::string buffer;
		
	std::ofstream outFile;
	std::ifstream mapSource( SOURCE_DIR "/sources/task1_5/input.txt" );
	
	if ( !mapSource.is_open() ) 
	{
		std::cerr << "Failed to open input file\n";
		return 1;
	}

	while ( std::getline(mapSource, buffer) )
	{
		map.push_back( buffer );
	}

	mapSource.close();
	
	int numberOfIslands = calculateIslands(map);

	outFile.open( SOURCE_DIR "/tests/task1_5/output.txt" );
	
	if ( !outFile.is_open() )
	{
		std::cerr << "Output File was not created\n";
		std::cout << "Number of islands is " << numberOfIslands << std::endl;
		return 1;
	}

	outFile << numberOfIslands << std::endl;

	outFile.close();

	return 0;
}

int calculateIslands( const std::vector< std::string >& map)
{
	int islandsNumber = 0;
	unsigned lineSize;
	if ( map.size() > 0 )
	{
		lineSize = map.at(0).length();		
	} 
	else 
	{
		return 0;
	}

	bool **isPartVisited = new bool*[map.size()];
	for ( size_t i = 0; i < map.size(); i++ )
	{		
		isPartVisited[i] = new bool[lineSize];
		for ( unsigned j = 0; j < lineSize; j++ )
		{
			isPartVisited[i][j] = false;
		}
	}

	
	// find coast and explore an island
	for ( size_t i = 0; i < map.size(); i++ )
	{		
		for ( size_t j = 0; j < lineSize; j++ )
		{
			char part = map.at(i).at(j);
			if ( part == 'o' && !isPartVisited[i][j] )
			{
				islandsNumber++;
				exploreIsland(i, j, map, isPartVisited);
			}
		}
	}

	for ( size_t i = 0; i < map.size(); i++ )
	{
		delete [] isPartVisited[i];
	}
	delete [] isPartVisited;
	
	return islandsNumber;
}
void exploreIsland( unsigned coordX, unsigned coordY, const std::vector< std::string >& map, bool** isVisited)
{
	isVisited[coordX][coordY] = true;

	if ( coordX - 1 >= 0 && map.at(coordX - 1).at(coordY) == 'o'
		&& !isVisited[coordX - 1][coordY] )
	{
		exploreIsland( coordX - 1, coordY, map, isVisited );
	}

	if ( coordY + 1 < map.at(0).length()  && map.at(coordX).at(coordY + 1) == 'o'
		&& !isVisited[coordX][coordY + 1] )
	{
		exploreIsland( coordX, coordY + 1, map, isVisited );
	}

	if ( coordX + 1 < map.size()  && map.at(coordX + 1).at(coordY) == 'o'
		&& !isVisited[coordX + 1][coordY] )
	{
		exploreIsland( coordX + 1, coordY, map, isVisited);
	}

	if ( coordY - 1 >= 0 && map.at(coordX).at(coordY - 1) == 'o'
		&& !isVisited[coordX][coordY - 1] )
	{
			exploreIsland( coordX, coordY - 1, map, isVisited);
	}
}