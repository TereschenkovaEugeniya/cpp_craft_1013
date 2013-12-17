#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <list>

using namespace std;

static const char ground = 'o';
static const char water  = '~';
static const char drowned = 'x';

//coordinates on map (row, col)
struct coords
{
	vector<string>::size_type row;
	string::size_type col;
};
//somewhy constructor doesnt work understandible way
coords get_coords(vector<string>::size_type r, string::size_type c)
{
	coords res;
	res.row = r;
	res.col = c;
	return res;
}

coords find_ground(const vector<string>& landmap)
{
	for(vector<string>::size_type i = 0; i<landmap.size(); ++i)
		for(string::size_type j = 0; j<landmap[0].size(); ++j)
			if(landmap[i][j]==ground)
			{
				return get_coords(i, j);
			}
	return get_coords(landmap.size(), landmap[0].size());
}

void drown_isle(vector<string>& landmap, coords starting_point)
{
	queue<coords> myqueue;
	myqueue.push(starting_point);
	coords cur;
	while(!myqueue.empty())
	{
		cur = myqueue.front();
		myqueue.pop();
		if(landmap[cur.row][cur.col]==ground)
		{
			landmap[cur.row][cur.col]=drowned;
			if(cur.row!=0)
				myqueue.push(get_coords(cur.row-1, cur.col));
			if(cur.col!=0)
				myqueue.push(get_coords(cur.row, cur.col-1));
			if(cur.row!=landmap.size()-1)
				myqueue.push(get_coords(cur.row+1, cur.col));
			if(cur.col!=landmap[0].size()-1)
				myqueue.push(get_coords(cur.row, cur.col+1));
		}
	}
}

int main( int argc, char* argv[] )
{
	ifstream input_file( BINARY_DIR "/input.txt");
	ofstream output_file( BINARY_DIR "/output.txt");
	if ( !input_file.is_open())
	{
		cerr << "input file does not exist" << endl;
		return 1;
	}
	vector<string> land;
	string line;
	while( input_file >> line)
	{
		land.push_back(line);
	}
	input_file.close();

	int isle_num = 0;
	coords cur;
	while( cur = find_ground(land), cur.row!=land.size())
	{
		++isle_num;
		drown_isle(land, cur);
	}
	output_file << isle_num << endl;
	output_file.close();
}

