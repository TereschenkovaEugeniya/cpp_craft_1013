#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

std::string removeOddSymbols(const std::string &text, const std::vector <char> &symbol)
{
	std::string ret(text);

	int count = symbol.size();
	for (int i = 0; i < count; ++i)
	{
		ret.erase(std::remove(ret.begin(), ret.end(), symbol[i]), ret.end());
	}
	return ret;
}

int main()
{
	std::ifstream inputFile(SOURCE_DIR "/input_inverted_rows.txt");
	std::ofstream outputFile(SOURCE_DIR "/output_inverted_rows.txt" );
	
	std::string text;
	std::getline(inputFile, text);
	std::transform(text.begin(), text.end(), text.begin(), tolower);

	std::string key;
	while (std::getline(inputFile, key)) 
    {
		if( key.length()==0)
			continue;

		std::vector<char> symbol;// = {' ', '\\', '-'};
		symbol.push_back(' ');
		symbol.push_back('-');
		symbol.push_back('\\');
		key = removeOddSymbols(key, symbol);

		std::reverse(key.begin(), key.end());
		std::transform(key.begin(), key.end(), key.begin(), tolower); 

		if (text.find(key) != std::string::npos)
			outputFile << std::string("YES") << std::endl;
		else
			outputFile << std::string("NO") << std::endl;			
    }

	inputFile.close();
	outputFile.close();
    
	system("pause");
	return 0;
}