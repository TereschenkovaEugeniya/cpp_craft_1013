#include <iostream>
#include <fstream>
#include <set> 
#include <stdint.h> 

int64_t GetIndex(double t)
{
    return static_cast<int64_t>(t*10000.0);
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
    int num = 0;
    in >> num;
    double code;
    typedef std::set<int64_t> SetInt64;
    SetInt64 code_set;
    for (int i=0; i < num; ++i)
    {
        in >> code;        
        code_set.insert(GetIndex(code));               
    }

    while (in >> code)
    {
        out << ((code_set.find(GetIndex(code)) != code_set.end()) ? "YES" : "NO") << std::endl;
    }
	return 0;
}

