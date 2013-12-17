#include <string>
#include "islands.h"

size_t check_size_file(ifstream &file, int &count_land)
{
	size_t size;
	string line;
	while(!file.eof())
	{
		getline(file, line);
		count_land++;
	}
	size = line.size();
	file.clear();
	file.seekg(0);
	return size;
}

vector<char>* read_file(ifstream &file, int &count_land)
{
	size_t size_land;
	count_land = 0;
	vector<char> *land;
	size_land = check_size_file(file, count_land);
	// to surround the perimeter of the islands with water
	count_land += 2;
	size_land += 2;

	land = new vector<char> [count_land];
	for(int i = 0; i < size_land; i++)
	{
		land[0].push_back(water);
	}
	for(int i = 1; i < count_land-1; i++)
	{
		land[i].push_back(water);
		for(int j = 1; j < size_land-1; j++)
		{
			land[i].push_back(file.get());
		}
		file.get();
		land[i].push_back(water);
	}
	for(int i = 0; i < size_land; i++)
	{
		land[count_land-1].push_back(water);
	}
	return land;
}

int island(vector<char>* land, int &count_land)
{
	int isl = 0;
	for(int i = 1; i < count_land-1; i++)
	{
		for(size_t j = 1; j < land[i].size(); j++)
		{
			if(land[i].at(j) == earth)
			{
				isl++;
				check_island(land, i, j);
			}
		}
	}
	return isl;
}

void check_island(vector<char>* land, int i, int j)
{
	if(land[i].at(j) == water)
		return;
	// change the water to earth for counted islands
	land[i].at(j) = water;

	check_island(land, i+1, j);
	check_island(land, i-1, j);
	check_island(land, i, j+1);
	check_island(land, i, j-1);
}