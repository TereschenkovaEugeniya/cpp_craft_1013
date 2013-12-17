#include <iostream> 
#include <fstream>
#include <string> 
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

void IdentifyIsland(vector<string> &all_map, int row , int col)
{
	queue<pair<int, int>> islands;
	islands.push(make_pair(row,col));
	int dx[] = {0, 1, 0, -1};
	int dy[] = {-1, 0, 1, 0};
	int max_row = all_map.size();
	int max_col = all_map[0].size();

	all_map[row][col] = 'x';
	while(!islands.empty())
	{
		for(int i = 0; i < 4; i++)
		{
			int new_col, new_row;
			new_row = islands.front().first + dx[i];
			new_col = islands.front().second + dy[i];

			if((0 <= new_row) && (new_row < max_row))
			{
				if((0 <= new_col) && (new_col < max_col))
				{
					if(all_map[new_row][new_col] == 'o')
					{
						all_map[new_row][new_col] = 'x';
						islands.push(make_pair(new_row, new_col));
					}	
				}
			}
		}
		islands.pop();
	}
}


int main() 
{ 
	string line;
	vector<string> all_map;
	fstream input_file, output_file;
	int count_of_islands = 0;

	input_file.open("Input.txt", fstream::in);
	
	if(input_file)
	{
	
		input_file >> line;
		all_map.push_back(line);
		// Read all map
		for(;!input_file.eof();)
		{
			input_file >> line;
			all_map.push_back(line);
		}
		input_file.close();

		// Find all islands and identify 
		for(int i = 0; i < all_map.size(); i++)
		{
			for(int j = 0; j < all_map[0].size(); j++)
			{
				if(all_map[i][j] == 'o')
				{
					IdentifyIsland(all_map, i, j);
					count_of_islands++;
				}
			}
		}

		output_file.open("Output.txt", fstream::out);
		output_file << count_of_islands << endl;
		output_file.close();
	}
}