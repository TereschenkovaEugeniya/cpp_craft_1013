#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

/*
** removes from s all occuraces of letters in t
*/
void strip_all(string& s, const string& t)
{
	string::size_type n = t.length();
	string::size_type i,j,k;
	for(i=0, j=0; i<s.length(); ++i)
	{
		for(k=0; k<n; k++)
			if(s[i]==t[k])
				break;
		if(k==n)
			s[j++]=s[i];
	}
	s.replace(j,s.npos,"");
}

void to_lower(string& s)
{
	//transform(s.begin(), s.end(), s.begin(), tolower);
	for(string::size_type i = 0; i<s.length(); i++)
		s[i] = tolower(s[i]);  
	//TODO tolower(s[i], locale) and #include <locale>
}

int main( int argc, char* argv[] )
{
	ifstream input_file( BINARY_DIR "/input.txt");
	ofstream output_file( BINARY_DIR "/output.txt");
	if( !input_file.is_open())
	{
		cerr << "input file does not exist" << endl;
		return 2;
	}
	string text, key;
	string not_In_Account_Symbols = " -\\";
	if( !input_file.eof())
	{
		getline( input_file, text);
		strip_all(text, not_In_Account_Symbols);
		to_lower(text);
		reverse(text.begin(), text.end());
	}else
	{
		cerr << "input file is empty" << endl;
		return 1;
	}
	while( !input_file.eof() )
	{
		getline( input_file, key );
		if( key.length()==0)
			break;
		strip_all(key, not_In_Account_Symbols);
		to_lower(key);
		if( text.find(key)!=string::npos)
			output_file << "YES" << endl;
		else
			output_file << "NO" << endl;
	}
	input_file.close();
	output_file.close();
}

