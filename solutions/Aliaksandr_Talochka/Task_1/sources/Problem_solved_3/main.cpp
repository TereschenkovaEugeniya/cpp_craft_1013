
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


void search_and_flood(vector<string>* map_ptr, int x, int y)
{
	if((*map_ptr)[y][x] == 'o')
	{
		(*map_ptr)[y][x] = '~';
		search_and_flood(map_ptr, x+1, y);
		search_and_flood(map_ptr, x-1, y);
		search_and_flood(map_ptr, x, y+1);
		search_and_flood(map_ptr, x, y-1);

	}
}

int main(int argc, char* argv[])
{
	
	std::fstream InFile;
	std::fstream OutFile;
	std::string InputFileName, LineStr;
	std::string* Water;
	int xsize, ysize;
	int num_islands = 0;
	vector<string> map;
	vector<string>::iterator it;

	if (argc==2)
		InputFileName = argv[1];
	else
		InputFileName = BINARY_DIR"/Input.txt";

	InFile.open(InputFileName.c_str(), std::ios_base::in);
	OutFile.open(BINARY_DIR"/Output.txt", std::ios_base::out|std::ios_base::trunc);

	if(InFile)
	{
		getline(InFile, LineStr);
		xsize = LineStr.length() + 2;
		char c = '~';

		Water = new string(xsize, c);
		map.push_back(*Water);	
		map.push_back('~' + LineStr + '~');

		while(getline(InFile, LineStr))
			map.push_back('~' + LineStr + '~'); 
		map.push_back(*Water);

		delete Water;

		
		for (unsigned int y = 0; y < map.size(); y++)
			for(unsigned int x = 0; x < xsize; x++)
			{
				if(map[y][x] == 'o')
				{
					num_islands++;
					search_and_flood(&map, x, y);
				}
			}


			OutFile << num_islands << endl;	
			
	}
	else
		std::cout << "Nothing is done. There is no file with the name " << InputFileName.c_str() << std::endl;

	InFile.close();
	OutFile.close();

	return 0;
		
}

