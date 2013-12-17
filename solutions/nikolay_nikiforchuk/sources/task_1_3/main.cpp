#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
using namespace std;

bool IsDelimeter (char symbol){
    return symbol == ' ' || symbol == '-' || symbol == '\\';
}

string prepareString (const std::string& str){
    std::string result;
    result.resize (str.size ());
    std::transform (str.begin(), str.end(), result.begin(), ::tolower);
    result.erase (remove_if (result.begin (), result.end (), IsDelimeter), result.end ());
    const int size = result.size ();
    if (!result.empty() && result[size - 1] == '\r')
        result.erase(size - 1);
    return result;
}


void writeResult(bool result, std::fstream& resultFile){
    resultFile << (result ? "YES" : "NO") << std::endl;
}

bool extractStringAndKeysFromFile(std::fstream& resultFile){
    std::ifstream input_file("input.txt");
    std::string line;
    std::string str;
    int count = 0;

    if(!input_file.is_open()){
        cout << "Can't open file " << endl;
        return false;
    }

    while(!input_file.eof()){
        count++;
        std::getline(input_file, line);
        if(count == 1){
            str = prepareString (line);
            std::reverse (str.begin (), str.end ());
        }
        else{
            string key = prepareString (line);
            writeResult((str.find (key) == string::npos ? false : true), resultFile);
        }
    }
    input_file.close();
    return true;
}

int main(){
    std::fstream resultFile( "output.txt", std::fstream::out | std::fstream::trunc );
    if(!resultFile.is_open()){
        cout << "Can't open file to write a result" << endl;
    }
    setlocale(LC_ALL, "Russian");
    extractStringAndKeysFromFile(resultFile);
    resultFile.close();
}




