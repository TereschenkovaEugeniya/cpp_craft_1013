#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>

using namespace std;

static const char* InputFilename = "Input.txt";
static const char* OutputFilename = "Output.txt";
static const int N = 1000000;

void ReadCodes(ifstream& ifs, int& codes_count, vector<double>& codes);
void ReadPasswords(ifstream& ifs, vector<double>& passwords);
void CheckPasswordsAndLogResults(const vector<double>& passwords,
                                 const vector<double>& codes,

                                 ofstream& ofs);
int main() {
  int codes_count = 0;
  vector<double> codes;
  vector<double> passwords;

  ifstream ifs(InputFilename);
  if (!ifs.good()) {
    cerr << "Can't open file: " << InputFilename << endl;
    return -1;
  }

  try {
    ReadCodes(ifs, codes_count, codes);
  } catch (exception& ex) {
    cerr << "ReadCodes  failed. Reason: " << ex.what() << endl;
    return -1;
  }
  if (codes_count != static_cast<int>(codes.size())) {
    cerr << "Wrong input data: \n";
    cerr << "Number of Codes : " << codes_count << endl
      << static_cast<int>(codes.size()) << " codes have been read, "
      << " maximal available number = " << N << endl;
    return -1;
  }

  ReadPasswords(ifs, passwords);
  if (passwords.size() == 0) {
    cerr << "Wrong input data: \n"
      << "There are no passwords in file " << InputFilename << endl;
    return -1;
  }

  ifs.close();

  ofstream ofs(OutputFilename);
  if (!ofs.good()) {
    cerr << "Can't create file: " << OutputFilename << endl;
    return -1;
  }

  CheckPasswordsAndLogResults(passwords, codes, ofs);
  ofs.close();
  
  return 0;
}


void ReadCodes(ifstream& ifs, int& count, vector<double>& codes) {
  using namespace boost;

  codes.clear();
  count = 0;
  string line;
  stringstream msg;

  getline(ifs, line);
  try {
    count = lexical_cast<int>(line.c_str());
  } catch (bad_lexical_cast& ex) {
    msg << "boost::bad_lexical_cast has been caught "
        << "when " << line << " had been casted to int. "
        << "Details: " << ex.what();
    throw logic_error(msg.str());
  }

  if (count > N) {
    msg << "The codes counter " << count
        << " is greater than allowed value.";
    throw logic_error(msg.str());
  }

  codes.reserve(count);
  for (int i = 0; i < count && !ifs.eof(); ++i) {
    getline(ifs, line);
    double code_value = 0.0;
    try {
      code_value = lexical_cast<double>(line.c_str());
    } catch (bad_lexical_cast& ex) {
      cerr << "ReadCodes: bad_lexical_cast has been caught, "
           << "when " << line << " had been casted to double. "
           << "Value will be skipped. "
           << "Details: " << ex.what() << endl;
      continue;
    }
    codes.push_back(code_value);
  }
}
void ReadPasswords(ifstream& ifs, vector<double>& passwords) {
  using namespace boost;

  passwords.clear();

  string line;
  while (!ifs.eof()) {
    getline(ifs, line);
    if (line.size() > 0) {
      double pass_value = 0.0;
      try {
        pass_value = lexical_cast<double>(line.c_str());
      } catch (bad_lexical_cast& ex) {
        cerr << "ReadPasswords: bad_lexical_cast has been caught, "
             << "when " << line << " had been casted to double. "
             << "Value will be skipped. "
             << "Details: " << ex.what() << endl;
        continue;
      }
      passwords.push_back(pass_value);
    }
  } // while (!ifs.eof())
}
void CheckPasswordsAndLogResults(const vector<double>& passwords,
                                 const vector<double>& codes, ofstream& ofs) {

  const double kPrecision = 0.0001;
  const double kScale = 1.0 / kPrecision;

  for (size_t i = 0; i < passwords.size(); ++i) {
    size_t j = 0;
    while (j < codes.size()) {
      if (::fabs(passwords[i] - codes[j]) < kPrecision) {
        double x = ::floor(::fabs(passwords[i]) * kScale);
        double y = ::floor(::fabs(codes[j]) * kScale);
        if (::fabs(x - y) == 0.0)
          break;
      }
      ++j;
    } // while (j < codes.size())
    if (j < codes.size())
      ofs << "YES";
    else
      ofs << "NO";
    ofs << endl;
  } // for (size_t i = 0; i < passwords.size(); ++i)
}
