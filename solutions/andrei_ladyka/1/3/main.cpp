/* 
 * File:   main.cpp
 * Author: user
 *
 * 1.3. Задача «Перевёрнутые строки».
 * Входные данные: файл input.txt. Выходные данные: файл output.txt.
 * 1ая строка файла содержит текст. Длина строки не более 100000 символов.
 * Все следующие строки файла содержат текстовые ключи.
 * Для каждого текстового ключа выяснить, есть ли ключ в тексте в перевёрнутом
 * виде (без учёта символов пробела “ “, тире “-“ и слеша “\”; все заглавные
 * символы равноценны строчным). Для каждого ключа требуется выдать «YES»
 * если он содержится в тексте и «NO» если не содержится. Ответ требуется
 * дать выходной файл в том же порядке, в котором ключи расположены
 * во входном файле.
 * 
 * Created on чацвер, 17, кастрычніка 2013, 22.09
 */

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdlib>

#define MAX_LINE_SIZE 100000

std::string washString(std::string & str) 
{
    std::string retString;
    int lenght = str.length();
    for (int i = 0 ; i< lenght; i++) 
    {
        char a = str.at(i);
        if ((a != ' ') && (a != '-') && (a != '\\'))
        {
            retString.push_back(std::tolower(a));
        }
    }
    
    return retString;

}

bool puzzle(std::string line)
{
    std::string newLine = washString(line);
    int lenght = newLine.length();
    int lenght2 = lenght/2;
    for (int i = 0 ; i<lenght2 ; i++)
    {
        if (newLine.at(i) == newLine.at(lenght-i-1)) {
            //OK
        } else {
            return false;
        }
    }
    return true;
    
}

void printToOutput(std::ofstream & outputfile , std::string msg) {
    outputfile << msg <<"\n";
    std::cout << msg << std::endl;
}

void task1p3(std::ifstream & inputfile, std::ofstream & outputfile)
{
    std::string line;
    while ( getline (inputfile,line) )
    {
        if (line.size() > MAX_LINE_SIZE) 
        {
            printToOutput(outputfile,"LINE is Big!!");
        }
        if (puzzle(line)) 
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
        task1p3(inputfile,outputfile);
        inputfile.close();
        outputfile.close();
    } else {
        std::cout << "fail" << std::endl;
    }
    
    return 0;
}

