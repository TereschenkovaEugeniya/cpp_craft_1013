#include <fstream>
#include <string>
#include <vector>
#include <queue>


void RemoveIslandFromMap(std::vector<std::string>& map, std::pair<size_t, size_t> o)
{
    std::queue<std::pair<size_t, size_t>> island;
    island.push(o);
	size_t i = o.first;
	size_t j = o.second;
	map[i][j] = ' ';

    while (!island.empty())
    {
        o = island.front();
        island.pop();

		i = o.first;
		j = o.second;


		if (j < map[i].length()-1 && map[i][j+1] == 'o')
		{
			island.push(std::make_pair(i,j+1));
			map[i][j+1] = ' ';
		}
		if (j > 0 && map[i][j-1] == 'o')
		{
			island.push(std::make_pair(i,j-1));
			map[i][j-1] = ' ';
		}
		if (i < map.size()-1 && map[i+1][j] == 'o')
		{
			island.push(std::make_pair(i+1,j));
			map[i+1][j] = ' ';
		}
		if (i > 0 && map[i-1][j] == 'o')
		{
			island.push(std::make_pair(i-1,j));
			map[i-1][j] = ' ';
		}
    }
}

int main()
{
	std::ifstream in(SOURCE_DIR "/input.txt");
	std::ofstream out(BINARY_DIR "/output.txt");

	if (in.is_open() && out.is_open())
	{
		std::vector<std::string> map;

		while (!in.eof())
		{
			std::string str;
			std::getline(in, str);
			map.push_back(str);
		}
    
		int count = 0;
		for (size_t i = 0; i < map.size(); ++i)
		{
			for (size_t j = 0; j < map[i].size(); ++j)
			{
				if (map[i][j] == 'o')
				{
					++count;
					RemoveIslandFromMap(map, std::make_pair(i, j));
				}
				else
				{
					map[i][j] = ' ';
				}
			}
		}

		out << count;
	}

    in.close();
    out.close();
}