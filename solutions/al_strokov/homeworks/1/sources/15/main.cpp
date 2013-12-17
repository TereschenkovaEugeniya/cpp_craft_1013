#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

typedef std::vector<std::string> landmap_t; //definition for map
static const char waterChar = '~';
static const char islandChar = 'o';

void renderMap(const landmap_t& map) {
	std::string s(map[0].length() + 2, '-');
	std::cout << s << std::endl;
	for (unsigned int i = 0; i < map.size(); i++) {
		std::cout << "|" << map[i] << "|" << std::endl;
	}
}

bool isCoordValid(const landmap_t& map, const unsigned int x,
		const unsigned int y) {
	return (x < map[0].length()) && (y < map.size());
}

char getLandChar(const landmap_t& map, const unsigned int x,
		const unsigned int y) {
	char ret = waterChar;

	if (isCoordValid(map, x, y)) {
		ret = map[y][x];
	}

	return ret;
}

void setLandChar(landmap_t& map, const unsigned int x, const unsigned int y,
		const char mark) {
	if (isCoordValid(map, x, y)) {
		map[y][x] = mark;
	}
}

bool isLand(const landmap_t& map, const unsigned int x, const unsigned int y) {
	return getLandChar(map, x, y) != waterChar;
}

int main(int argc, char** argv) {
	std::cout << "*homework 15*" << std::endl;

	std::ifstream inputFile(SOURCE_DIR "/sources/15/input.txt");
	std::ofstream outputFile(SOURCE_DIR "/sources/15/output.txt");

	if ( !inputFile.is_open() || !outputFile.is_open()){
		std::cout << "cannot open files" << std::endl;
		return 1;
	}

	size_t linesCount = 0;
	size_t lineLength = 0;
	std::string strBuff;
	landmap_t landmap;
	const std::string marks =
			"01234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	while (std::getline(inputFile, strBuff)) {
		landmap.push_back(strBuff);
	}
	linesCount = landmap.size();
	lineLength = landmap[0].length();

	renderMap(landmap);

	unsigned int landCount = 0;
	unsigned int correction = 0;
	char currMark = marks[0];

	//jump over lines
	for (unsigned int y = 0; y < linesCount; y++) {

		//jump inside line
		for (unsigned int x = 0; x < lineLength; x++) {

			if (isLand(landmap, x, y)) {

				currMark = marks[landCount];

				while (isLand(landmap, x + 1, y)) {

					if (isLand(landmap, x, y - 1)
							&& getLandChar(landmap, x, y - 1) != currMark) {
						correction++;
					}

					if (isLand(landmap, x, y - 1)) {
						currMark = getLandChar(landmap, x, y - 1);
					}

					setLandChar(landmap, x, y, currMark);
					x++;
				}

				if (isLand(landmap, x, y - 1)
						&& getLandChar(landmap, x, y - 1) != currMark) {
					correction++;
				}

				setLandChar(landmap, x, y, currMark);

				if (!isLand(landmap, x + 1, y)) {
					landCount++;
				}
			}
		}

		//normalize line
		for (unsigned int x = 0; x < lineLength; x++) {
			if (isLand(landmap, x, y)) {
				currMark = getLandChar(landmap, x, y);

				while (isLand(landmap, x + 1, y)) {
					setLandChar(landmap, x + 1, y, currMark);
					x++;
				}
			}
		}
	}

	renderMap(landmap);

	std::cout << " l:" << landCount << " c:" << correction << " rez:"
			<< landCount - correction << std::endl;

	outputFile << landCount - correction << std::endl;

	inputFile.close();
	outputFile.close();

	return 0;
}
