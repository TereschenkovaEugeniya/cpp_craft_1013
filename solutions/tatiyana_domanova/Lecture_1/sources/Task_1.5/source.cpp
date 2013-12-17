#include <fstream>
#include <vector>
#include <string>

using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;

void readData(ifstream &in, vector<vector<bool>*> &map) {
	string s;
	int row = 0;
	while (!in.eof()) {
		getline(in,s);
		map.push_back(new vector<bool> ());
		
		for (int i=0; i<s.length(); i++)
			if ( s[i] == '~') {
				map[row]->push_back(0);
			} else {
				map[row]->push_back(1);
			}
		row++;
	}
}

void markIsland(const vector<vector<bool>*> &map, int x, int y) {
	(*map[x])[y] = 0;
	if (x>0 &&  (*map[x-1])[y] == 1) 
		markIsland(map,x-1,y);
	if (x<map.size()-1 && (*map[x+1])[y] == 1) 
		markIsland(map,x+1,y);
	if (y>0 && (*map[x])[y-1] == 1) 
		markIsland(map,x,y-1);
	if (y<map[x]->size()-1 && (*map[x])[y+1] == 1) 
		markIsland(map,x,y+1);
}

int countIslands(const vector<vector<bool>*> &map) {
	int count = 0;
	for (int i=0; i<map.size(); i++)
		for (int j=0; j<map[i]->size(); j++)
			if ((*map[i])[j] == 1) {
				count ++;
				markIsland(map, i,j);
			}
	return count;
}

int main() {
	ifstream in("input.txt");
	ofstream out("output.txt");
	vector<vector<bool>*> map;
	readData(in,map);
	out << countIslands(map);
	for (int i=0; i<map.size(); i++) {
		delete map[i];
	}
	in.close();
	out.close();
	return 0;
}