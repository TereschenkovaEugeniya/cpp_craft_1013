#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>


using namespace std;

int main(int argc, char** argv)
{
	std::fstream InFile;
	std::fstream OutFile;
	std::string InputFileName, HeaderStr, KeyStr;

	if (argc==2)
	{
		InputFileName = argv[1];
	}
	else
	{
		InputFileName = BINARY_DIR"/Input.txt";
	}

	InFile.open(InputFileName.c_str(), std::ios_base::in);
	OutFile.open(BINARY_DIR"/Output.txt", std::ios_base::out|std::ios_base::trunc);

	if(InFile)
	{
		getline(InFile, HeaderStr);						//Read first string
		reverse(HeaderStr.begin(), HeaderStr.end());	//Reverse first string

		HeaderStr.erase(std::remove(HeaderStr.begin(), HeaderStr.end(), ' '), HeaderStr.end());		//delete symbols
		HeaderStr.erase(std::remove(HeaderStr.begin(), HeaderStr.end(), '-'), HeaderStr.end());
		HeaderStr.erase(std::remove(HeaderStr.begin(), HeaderStr.end(), '\\'), HeaderStr.end());
		transform(HeaderStr.begin(), HeaderStr.end(), HeaderStr.begin(), tolower);					//change register


		while(std::getline(InFile, KeyStr))
		{
			KeyStr.erase(std::remove(KeyStr.begin(), KeyStr.end(), ' '), KeyStr.end());
			KeyStr.erase(std::remove(KeyStr.begin(), KeyStr.end(), '-'), KeyStr.end());
			KeyStr.erase(std::remove(KeyStr.begin(), KeyStr.end(), '\\'), KeyStr.end());
			transform(KeyStr.begin(), KeyStr.end(), KeyStr.begin(), tolower);
			
			int pos = HeaderStr.find(KeyStr);						//Find key
			if(pos != string::npos)
			{
				OutFile << "YES" << endl;
			}
			else
			{
				OutFile << "NO" << endl;
			}

		}

	}
	else
	{
		std::cout << "Nothing is done. There is no file with the name " << InputFileName.c_str() << std::endl;
	}

	InFile.close();
	OutFile.close();

	return 0;
}