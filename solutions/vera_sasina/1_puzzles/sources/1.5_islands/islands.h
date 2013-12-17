#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

static const char water = '~', earth = 'o';

size_t check_size_file(ifstream&, int&);			// function count number of line in file
vector<char>* read_file(ifstream&, int&);		// function read information from file to vectors
int island(vector<char>*, int&);				// function count the number of 'islands'
void check_island(vector<char>*, int, int);		// 