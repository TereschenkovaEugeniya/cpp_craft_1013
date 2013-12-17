#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

static const char* InputFilename = "Input.txt";
static const char* OutputFilename = "Output.txt";

const char kLandPiece = 'o';
const char kWaterPiece = '~';

void ReadLandscape(ifstream& ifs, vector<string>& landscape);
void ExploreLandscape(const vector<string>& landscape_in, 
					  vector< vector<int> >& landscape_out);
int CountIslands(const vector< vector<int> >& landscape);

int main() {

  ifstream ifs(InputFilename);
  if (!ifs.good()) {
    cerr << "Can't open file: " << InputFilename << endl;
    return -1;
  }

  vector<string> raw_landscape;
  try {
    ReadLandscape(ifs, raw_landscape);
  } catch (const exception& ex) {
    cerr << "ReadLandscape failed. "
         << "Reason: " << ex.what() << endl;
    return -1;
  }
  ifs.close();

  vector< vector<int> > landscape;
  try {
    ExploreLandscape(raw_landscape, landscape);
  } catch (const exception& ex) {
    cerr << "ExploreLandscape failed. "
         << "Reason: " << ex.what() << endl;
    return -1;
  }
  
  ofstream ofs(OutputFilename);
  if (!ofs.good()) {
    cerr << "Can't create file: " << OutputFilename << endl;
    return -1;
  }

  try {
    ofs << CountIslands(landscape);
  } catch (const exception& ex) {
    cerr << "CountIslands failed. "
         << "Reason: " << ex.what() << endl;
    return -1;
  }

  ofs.close();

  return 0;
}

void ReadLandscape(ifstream& ifs, vector<string>& landscape) {
  landscape.clear();
  while (!ifs.eof()) {
    string line;
    getline(ifs, line);
    landscape.push_back(line);
  }
}
void ExploreLandscape(const vector<string>& landscape_in, 
                      vector< vector<int> >& landscape_out) {
  landscape_out.clear();

  size_t height = landscape_in.size();
  if (height == 0)
    return;

  size_t width = 0;
  for (size_t i = 0; i < landscape_in.size(); ++i) {
    if (landscape_in[i].length() > width)
      width = landscape_in[i].length();
    vector<int> id_row(landscape_in[i].length(), 0);
    landscape_out.push_back(id_row);
  }

  int land_id = 0;
  bool land = false;
  for (size_t i = 0; i < width; ++i) {
    if (landscape_in[height - 1][i] == kLandPiece) {
      if (land == false) {
        ++land_id;
        land = true;
      }
      landscape_out[height - 1][i] = land_id;
    } else if (landscape_in[height - 1][i] == kWaterPiece) {
      land = false;
    }
  }

  if (height == 1)
    return;

  size_t row = height - 2;
  while (row >= 0) {

    size_t col = 0;
    while (col < width) {
      if (landscape_in[row][col] == kLandPiece) {

        size_t lpos = col;
        while (col < width && landscape_in[row][col] == kLandPiece) ++col;
        size_t rpos = col;

        int id = 0;
        size_t col1 = lpos;
        while (col1 < rpos) {
          if (landscape_out[row + 1][col1] != 0) {
            id = landscape_out[row + 1][col1];
            break;
          }
          ++col1;
        } // while (col1 < rpos)

        if (col1 == rpos) {
          ++land_id;
          id = land_id;
        }

        for (col1 = lpos; col1 < rpos; ++col1)
          landscape_out[row][col1] = id;

      } // if (landscape_in[row][col] == kLandPiece)
      ++col;
    } // while (col < width)

    size_t row1 = row;
    while (row1 < height - 1) {
      col = 0;
      while (col < width) {
        if (landscape_out[row1][col] != 0 && landscape_out[row1 + 1][col] != 0 &&
            landscape_out[row1][col] != landscape_out[row1 + 1][col]) {
          size_t lpos = col;
          size_t rpos = col;
          while (lpos >= 0 && landscape_out[row1 + 1][lpos] != 0) --lpos;
          while (rpos < width && landscape_out[row1 + 1][rpos] != 0) ++rpos;
          ++lpos;
          while (lpos != rpos) {
            landscape_out[row1 + 1][lpos] = landscape_out[row1][col];
            ++lpos;
          }
        }
        ++col;
      } // while (col < width)

      ++row1;
    } // while (row1 < height - 2)

    if (row == 0)
      break;
    --row;
  } // while (row >= 0)
}
int CountIslands(const vector< vector<int> >& landscape) {
  set<int> lands_ids;
  for (size_t i = 0; i < landscape.size(); ++i)
    for (size_t j = 0; j < landscape[i].size(); ++j)
      if (landscape[i][j] != 0)
        lands_ids.insert(landscape[i][j]);
  return static_cast<int>(lands_ids.size());
}
