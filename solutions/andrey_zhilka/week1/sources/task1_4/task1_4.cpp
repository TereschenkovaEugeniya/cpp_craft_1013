#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>

void initArray( std::vector< double >&, std::ifstream& );
void processPasswords( const std::vector< double >&, std::ifstream& );
bool myBinarySearch( const std::vector< double >&, int, int, double );
bool find_equal( const std::vector< double >&, double );

const static double eps = 0.0001;

int main()
{
	std::vector<double> codes;	
	std::ifstream inputFile( SOURCE_DIR "/sources/task1_4/input.txt" );

	if ( !inputFile.is_open() ) 
	{
		std::cerr << "Failed to open input file" << std::endl;
		return 1;
	}	

	initArray( codes, inputFile );
	processPasswords( codes, inputFile);

	inputFile.close();
	return 0;
}

void initArray( std::vector< double >& codes, std::ifstream& inputFile ) 
{
	int N;
	double buffer;

	inputFile >> N;	

	for (int i = 0; i < N; i++) 
	{
		inputFile >> buffer;
		codes.push_back(buffer);	
	}

	sort(codes.begin(), codes.end());
}

void processPasswords( const std::vector<double>& codes, std::ifstream& inputFile ) 
{
	std::ofstream outFile( SOURCE_DIR "/tests/task1_4/output.txt" );

	if ( !outFile.is_open() ) 
	{
		std::cerr << "Failed to create output file" << std::endl;
		std::exit( 1 );
	}

	double nextPassword;

	while ( inputFile >> nextPassword )
	{
		if ( find_equal(codes, nextPassword) )
		{
			outFile << "YES\n";
		}
		else
		{
			outFile << "NO\n";
		}
	}

	outFile.close();
}

bool find_equal( const std::vector< double >& codes, double password) 
{
	return myBinarySearch( codes, 0, codes.size(), password );
}

bool myBinarySearch( const std::vector< double >& codes, int left,
					int right, double element )
{	
	if ( left >= right ) 
	{
		return false;
	}	

	int middle = left + ( right - left ) / 2;
	
	if ( left == right - 1 ) 
	{
		double difference = fabs( element ) - fabs( codes.at(left) );
		return (  difference < eps && difference >= 0);
	}
	
	if( element < 0 && element <= codes.at(middle)
		&& ( codes.at(middle) - element ) < eps )
	{
		return true;
	}
	else if ( codes.at(middle) <= element 
		&& ( element - codes.at(middle) ) < eps )
	{
		return true;		
	} 
	else if ( codes.at(middle) < element )
	{
		return myBinarySearch( codes, middle + 1, right, element );		
	}
	else 
	{
		 return myBinarySearch( codes, left, middle, element );
	}
}