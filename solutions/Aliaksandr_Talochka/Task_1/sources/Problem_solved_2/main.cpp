#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <math.h>


using namespace std;

int main(int argc, char** argv)
{
	std::fstream InFile;
	std::fstream OutFile;
	std::string InputFileName, HeaderStr, KeyStr, HeaderStrProc, KeyStrProc;

	int N;
	double tmp;
	vector<int> Codes;
	vector<int> Pass;
	std::vector<int>::iterator it;



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
		InFile >> N;						//Read quantity of codes
										
		for (int i = 0; i<N; i++)			//Read codes
		{
			InFile.precision(6);
			InFile >> tmp;
			Codes.push_back(floor(abs(tmp*10000)));
		}

		while(InFile >> tmp)				//Read pass
		{
			Pass.push_back(floor(abs(tmp*10000)));
		}


		for (int i=0; i<Pass.size(); i++)
		{

			//Check whether Pass is in the vector of Codes
			it = std::find(Codes.begin(), Codes.end(), Pass[i]);
			if(it == Codes.end())
			{
				OutFile << "NO" << endl;
			}
			else
			{
				OutFile << "YES" << endl;
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