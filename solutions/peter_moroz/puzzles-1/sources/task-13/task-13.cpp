#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <locale>

using namespace std;

static const char* InputFilename_translit = "Input_translit.txt";
static const char* InputFilename_ru1251 = "Input_ru1251.txt";
static const char* OutputFilename = "Output.txt";
static const size_t MaxTextLength = 100000;
static const char* Delimiters = " -\\";

inline void ToLower(string& s) {
  transform(s.begin(), s.end(), s.begin(), ::tolower);
}

/*class IfCharacterInString {
public:
  IfCharacterInString(const string& s) : s_(s) {}
  bool operator()(char x) {
    return (s_.find(x) != string::npos);
  }
private:
  string s_;
};*/

class IfCharacterInString : public unary_function<char, bool> {
public:
  IfCharacterInString(const string& s) : s_(s) {}
  bool operator()(char x) {
    return (s_.find(x) != string::npos);
  }
private:
  string s_;
};

inline void RemoveDelimiters(string& s, const string& delimiters) {
  IfCharacterInString if_character_in_string(delimiters);
  s.erase(remove_if(s.begin(), s.end(), if_character_in_string), s.end());
}

void ReadKeysWithReverse(ifstream& ifs, vector<string>& keys);
void FindKeysAndLogResult(const string& where,
                          const vector<string>& keys,
                          ofstream& ofs);

string Join(const string& s, const char* delims = " -\\");
vector<string> Split(const string& s, const char* delims = " -\\");


int main() {

  const char* InputFilename = InputFilename_ru1251;

  try {
    locale cp1251_locale("rus_rus.1251");
    locale::global(cp1251_locale);
  } catch (runtime_error&) {
    InputFilename = InputFilename_translit;
  }

  ifstream ifs(InputFilename);
  if (!ifs.good()) {
    cerr << "Can't open file: " << InputFilename << endl;
    return -1;
  }


  string discovered_text;
  getline(ifs, discovered_text);

  if (discovered_text.length() > MaxTextLength) {
    cout << "The length of discoverred text is greater than"
      << " maximal avalilable value (" << MaxTextLength << ")."
      << " Content of text has been truncated to this value.\n";
    discovered_text.resize(MaxTextLength);
  }

  ToLower(discovered_text);
  
  RemoveDelimiters(discovered_text, Delimiters);

  vector<string> keys;
  ReadKeysWithReverse(ifs, keys);
  ifs.close();

  ofstream ofs(OutputFilename);
  FindKeysAndLogResult(discovered_text, keys, ofs);
  ofs.close();

  return 0;
}

void ReadKeysWithReverse(ifstream& ifs, vector<string>& keys) {
  string line;
  while (!ifs.eof()) {
    getline(ifs, line);
    if (line.length() > 0) {
      ToLower(line);
      RemoveDelimiters(line, Delimiters);
      reverse(line.begin(), line.end());
      keys.push_back(line);
    }
  }
}
void FindKeysAndLogResult(const string& where, 
                          const vector<string>& keys, ofstream& ofs) {
  for (size_t i = 0; i < keys.size(); ++i) {
    if (where.find(keys[i]) == string::npos)
      ofs << "NO";
    else
      ofs << "YES";
    ofs << endl;
  } // for (size_t i = 0; i < keys.size(); ++i)
}
string Join(const string& s, const char* delims /*= " -\\"*/) {
  vector<string> parts = Split(s, delims);
  string result;
  for (size_t i = 0; i < parts.size(); ++i)
    result.append(parts[i]);
  return result;
}
vector<string> Split(const string& s, const char* delims /*= " -\\"*/) {
  vector<string> parts;
  size_t pos0 = 0;
  size_t pos1 = 0;
  while (pos0 != string::npos) {
    pos1 = s.find_first_of(delims, pos0);
    size_t len = pos1 != string::npos ? pos1 - pos0 : string::npos;
    parts.push_back(s.substr(pos0, len));
    if (pos1 == string::npos)
      break;
    pos0 = s.find_first_not_of(delims, pos1);
  }
  return parts;
}
