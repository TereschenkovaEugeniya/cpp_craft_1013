#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <vector>
#include <algorithm>

bool isEquals (double a, double b, double degree);
long long int myRound (double number, double degree);

int main()
{
    static const double degree = 10000;
    static const std::string yes = "YES";
    static const std::string no = "NO";
    long n = 1;
    double pass;

    //open
    std::ifstream inFile	(BINARY_DIR "input.txt");
    std::ofstream outFile	(BINARY_DIR "output.txt");

    inFile >> n;
    if (n <= 0 || !inFile.is_open()) {
        return 1;
    }

    double  *codes = new double [n];
    for (int i = 0; i<n; ++i){
        inFile >> codes[i];
    }

    //should be optimized for "std::binary_search"
    bool eqNum = false;
    while(true) {
        inFile >> pass;
        if(inFile.eof())
            break;
        eqNum = false;
        for(int i = 0; i < n; ++i){
            if(isEquals(pass, codes[i], degree)) {
                eqNum = true;
                break;
            }
        }
        outFile << (eqNum ? yes : no).c_str() << std::endl;
    }
    //close
    delete[] codes;
    outFile.close();
    inFile.close();
    return 0;
}


bool isEquals(double a, double b, double degree) {
    return myRound(a, degree) == myRound(b, degree);
}

long long int myRound (double number, double degree) {
    number *= degree;
    return static_cast<long long int>(number);
}
