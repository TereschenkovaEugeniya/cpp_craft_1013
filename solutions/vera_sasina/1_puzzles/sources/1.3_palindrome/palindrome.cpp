#include "palindrome.h"

using namespace std;

void  string_preparation(string& str)
{
	for(size_t i = 0; i < str.size(); i++)
	{
		if(isalpha(str.at(i)))
			str[i] = tolower(str.at(i));
		if(str.at(i) == ' ' || str.at(i) == '\\' || str.at(i) == '-')
			str.erase(i--, 1);
	}
}

bool find_key(const string& key, const string &str)
{
	string temp_key;
	 for( string::const_reverse_iterator it = key.rbegin(); it != key.rend(); ++it ) 
		 temp_key.push_back( *it );

	string_preparation(temp_key);

	return (str.find(temp_key) != string::npos);
}