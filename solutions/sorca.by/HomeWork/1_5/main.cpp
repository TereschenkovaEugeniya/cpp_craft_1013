#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <iterator>

int countingLength (std::ifstream& inFile, int position);
int countingWidht (std::ifstream& inFile, int position);
void atlantis (bool** world, const int& width, const int& length, const int& i, const int& j);

int main() {
	static const int zero = 0;
	static const char land = 'o';
	static const char water = '~';

	int countIslands = 0;

	std::ifstream inFile  ("input.txt");
	std::ofstream outFile ("output.txt");
	
	if (!inFile.is_open()) {
		return 1;
	}

 	static const int length = countingLength(inFile, zero);
 	static const int width = countingWidht(inFile, zero);
	inFile.clear();
	inFile.seekg(zero);

	bool** world = new bool* [width];
	for (int i = 0; i < width; ++i) {
		world [i] = new bool [length];
		for (int j = 0; j < length; ++j) {
			world [i][j] = (inFile.get() == land);
		}
		inFile.get();                  //can be compared to the \n in the cycle
	}

	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < length; ++j) {
			if(world [i][j]) {
				atlantis(world, width, length, i, j);
				countIslands++;
			}
		}	
	}

	outFile << countIslands << std::endl;

	for (int i = 0; i < width; ++i)	{
		delete [] world [i];
	}
	delete [] world;

	inFile.close();
	outFile.close();
	return 0;
}

int countingLength (std::ifstream& inFile, int position) {
	std::string line;
	inFile.clear();
	inFile.seekg(position);
	std::getline(inFile, line);
	inFile.clear();
	inFile.seekg(position);
	return static_cast<int>(line.size());
}

int countingWidht(std::ifstream& inFile, int position) {
		inFile.clear();
		inFile.seekg(position);
	return static_cast<int>(std::distance(
		(std::istream_iterator<std::string>(inFile)),
		(std::istream_iterator<std::string>())
		));
}

void atlantis(bool** world, const int& width, const int& length, const int& i, const int& j) {
	if ( !world [i][j] ) {
		return;
	}

	static const int one = 1;
	world [i][j] = false;

	//left
	if (j>0) {
		atlantis(world, width, length, i, j-one);
	}

	//right
	if (j < length-one) {
		atlantis(world, width, length, i, j+one);
	}

	//up 
	if (i >0) {
		atlantis(world, width, length, i-one, j);
	}

	//down
	if (i < width-one) {
		atlantis(world, width, length, i+one, j);
	}

	return;
}

