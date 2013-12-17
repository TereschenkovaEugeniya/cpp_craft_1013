#include <fstream>
#include <string>
#include <algorithm> 


void PrepareStr(std::string& str)
{
    str.erase(std::remove_if(str.begin(), str.end(), [](char a){return (a == ' ') || (a == '\\') || (a == '-');}), str.end());
	std::transform(str.begin(), str.end(), str.begin(), toupper);
}

int main()
{
    setlocale(LC_ALL, "");

	std::ifstream in(SOURCE_DIR "/input.txt");
	std::ofstream out(BINARY_DIR "/output.txt");

	std::string text, key;
    std::getline(in, text);
	PrepareStr(text);

	if (in.is_open() && out.is_open())
	{
		while(!in.eof())
		{
			std::getline(in, key);
			PrepareStr(key);
        
			std::reverse(key.rbegin(), key.rend());
			if(text.find(key) != std::string::npos)
			{
				out << "YES" << std::endl;
			}
			else
			{
				out << "NO" << std::endl;
			}
		}
	}

    in.close();
    out.close();
}