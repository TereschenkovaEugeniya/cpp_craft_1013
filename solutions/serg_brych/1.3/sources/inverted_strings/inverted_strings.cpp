#include <iostream> 
#include <fstream>
#include <string> 
#include <functional>
#include <algorithm>
#include <cctype>
#include <locale>

// Предикат возвращает true вслучае если найденный символ является пробелом, дефисов или слешом
bool Pred(char ch)
{
	return ch == ' ' || ch == '-' || ch == '\\';
}

int main() 
{ 
	std::fstream input_file, output_file;
	std::string text, str;
	input_file.open("Input.txt", std::fstream::in);
	if(input_file)
	{
		output_file.open("Output.txt", std::fstream::out);
		std::getline(input_file, text);
		std::transform(text.begin(), text.end(), text.begin(), std::bind2nd(std::ptr_fun(&std::tolower<char>), std::locale("")));
		text.erase(std::remove_if(text.begin(), text.end(), Pred), text.end());
		std::string revers_string(text.rbegin(), text.rend());
		std::getline(input_file, str);
		while(!input_file.eof())
		{
			std::transform(str.begin(), str.end(), str.begin(), std::bind2nd(std::ptr_fun(&std::tolower<char>), std::locale("")));
			str.erase(std::remove_if(str.begin(), str.end(), Pred), str.end());

			if(std::search(text.begin(), text.end(), str.begin(), str.end())!= text.end())
			{
				output_file << "YES" << std::endl;
			}
			else
			{
				output_file << "NO" << std::endl;
			}	
			std::getline(input_file, str);
		}

		input_file.close();
		output_file.close();
	}
}