/* 
 * File:   main.cpp
 * Author: user
 * 
 * Входные данные: файл input.txt. Выходные данные: файл output.txt.
 * Файл содержит два массива рациональных чисел, массив кодов и массив паролей,
 * заданных следующим образом: Сначала в файле находится целое число
 * N – количество рациональных значений первого массива. Затем N рациональных
 * значений массива кодов. Затем идут M рациональных значений массива
 * паролей (до конца файла). N <= 1000000. Для каждого значения массива
 * паролей выдать в выходной файл “YES” если существует код идентичный
 * данному паролю и “NO” если не существует. Запись в выходной файл
 * произвести в том же порядке, в котором находились значения массива
 * паролей. Все рациональные значения имеют точность 10^(-4).
 * 
 * Created on чацвер, 18, кастрычніка 2013, 11.28
 */

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>

#define ACCURACY 10000


void printToOutput(std::ofstream & outputfile , std::string msg) {
    outputfile << msg <<"\n";
    std::cout << msg << std::endl;
}

std::vector<int64_t> readPasswd(std::ifstream & inputfile) {
    std::vector<int64_t> passwd;
    int count;
    inputfile >> count;
    passwd.reserve(count);
    double value;
    for(int i=0; i < count && !inputfile.eof(); i++){
        inputfile >> value;
        passwd.push_back((int64_t)(value*ACCURACY));
    }    
    return passwd;
}

bool searchfind (std::vector<int64_t> & passwd, double & value) 
{
    return (std::binary_search(passwd.begin(), passwd.end(),(int64_t)(value*ACCURACY)));
}
void task1p4(std::ifstream & inputfile, std::ofstream & outputfile)
{
    std::vector<int64_t> passwd = readPasswd(inputfile);
    std::sort(passwd.begin(), passwd.end());
    double value;
    while(!inputfile.eof()){
        inputfile >> value;
        if (searchfind(passwd,value)) 
        {
            printToOutput(outputfile,"YES");
        } else {
            printToOutput(outputfile,"NO");
        }

    }
}

bool openFiles(std::ifstream & inputfile, std::ofstream & outputfile) 
{
    return ( (inputfile.is_open()) && (outputfile.is_open()));
}

int main(int argc, char** argv) 
{   
    std::ifstream inputfile (SOURCE_DIR "/input.txt");
    std::ofstream outputfile (SOURCE_DIR "/output.txt");
    if (openFiles(inputfile,outputfile)) {
        task1p4(inputfile,outputfile);
        inputfile.close();
        outputfile.close();
    } else {
        std::cout << "fail" << std::endl;
    }
    
    return 0;
}

