#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

bool hasOldIsland(int row, int column, const std::vector <std::string> &earth)
{
	size_t countColumn = earth[row].size();
	for (size_t j = column; j < countColumn; ++j)
	{
		if ((j + 1) < countColumn && earth[row][j + 1] == 'o')
		{
			if (row > 0 && earth[row-1][j + 1] == 'o')
			{
				return true;
			}
		}
		else 
		{
			return false;
		}
	}
	return false;
}

int main()
{
	std::ifstream inputFile(SOURCE_DIR "/input.txt");
	std::ofstream outputFile(SOURCE_DIR "/output.txt" );
	
	if ( inputFile && outputFile)
	{
		std::vector <std::string> earth;
		int countIslands = 0;

		while (!inputFile.eof()) 
		{
			std::string pieceOfEarth;
			std::getline(inputFile, pieceOfEarth);
			
			if (pieceOfEarth.empty())
				continue;

			earth.push_back(pieceOfEarth);
		}

		size_t countPieceEarth = earth.size();
		for (size_t row = 0; row < countPieceEarth; ++row)
		{
			int countItems = earth[row].size();
			for (int column = 0; column < countItems; ++column)
			{
				if (earth[row][column] == 'o')
				{
					if ((row <= 0 || earth[row - 1][column] != 'o') 
						&& (column <= 0 || earth[row][column - 1] != 'o'))
					{
						if (countPieceEarth == 1)
						{
							++countIslands;
						} else
						{
							if (!hasOldIsland(row, column, earth))
							{
								for (size_t offset = 1; offset < countPieceEarth; ++offset)
								{	
									if ((row + offset) >= countPieceEarth 
										|| earth[row + offset][column] != 'o' 
										|| !hasOldIsland(row + offset, column, earth)) 
									{
										++countIslands;
										break;
									} else
									{
										break;
									}
								}
							}
						}
					}
				}
			}
		}
		std::stringstream countIslandString;
		countIslandString << countIslands;

		outputFile << countIslandString.str() << std::endl;

		inputFile.close();
		outputFile.close();
	}
	return 0;
}