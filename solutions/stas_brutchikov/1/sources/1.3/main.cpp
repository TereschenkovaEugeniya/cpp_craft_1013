#include <string>
#include <iostream>
#include <fstream>
#include <algorithm> 

void PrepareString(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    str.erase(std::remove_if (str.begin(), str.end(), [](char ch)->bool
        {
            static std::string signs(" -\\");
            return (signs.find(ch) != std::string::npos);
        }), str.end());
}

int main()
{
    setlocale(LC_ALL, "");
    std::ifstream in (BINARY_DIR "/Input.txt");
    std::ofstream out (BINARY_DIR "/Output.txt");
	if (!in.is_open()|| !out.is_open())
    {
        return 1;
    }
    std::string base_str;
    std::getline(in, base_str);
    PrepareString(base_str);
    while (!in.eof())
    {
        std::string str;        
        std::getline(in, str);
        if(!str.empty())
        {
            PrepareString(str);
            std::reverse(str.begin(), str.end());
            out << ((base_str.find(str)!=std::wstring::npos) ? "YES" : "NO") << std::endl;
        }
    }
	return 0;
}

