/* 
 * File:   main.cpp
 * Author: user
 *
 * 1.5. Задача “Острова».
 * Входные данные: файл input.txt. Выходные данные: файл output.txt.
 * В файле input.txt находится карта земли, где: 'o' символ –суша, '~' – вода.
 * Требуется в файл output.txt выдать количество островов на карте (части 
 * суши соединённые, если они имеют общую границу по горизонтали или вертикали).
 * Гарантируется, что карта имеет одинаковое количество символов в каждой строке.
 * 
 * Created on чацвер, 18, кастрычніка 2013, 12.05
 */

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <vector>

void getBoolRow (std::vector<bool*> & map, const std::string & line, const int & linesize) {
    bool row[linesize];
    for (int i = 0 ; i< linesize; i++) 
    {
        //Вода false
        //Земля true
        bool a = (line.at(i) != 'o');
        row[i] = a;
    }
    
    map.push_back(row);
}
void getBoolMap (std::ifstream & inputfile,std::vector<bool*> & map,const int & linesize) {
    std::string line;
    while ( getline (inputfile,line) )
    {
        getBoolRow(map,line,linesize);
    } 
}

bool sink(std::vector<bool*> & map, const int & i, const int & j, const int & is, const int & linesize) 
{
    bool * a = map.at(i);
    if (*(a+j)) 
    {
        *(a+j) = false;
        if (i+1 < is) 
        {
            sink(map,i+1,j,is,linesize);
        }
        if (j+1 < linesize) 
        {
            sink(map,i,j+1,is,linesize);
        }
        
        if (i-1 >= 0) 
        {
            sink(map,i-1,j,is,linesize);
        }
        if (j-1 >= 0) 
        {
            sink(map,i,j-1,is,linesize);
        }
        return true;
    } else {
        return false;
    }
    
}
void task1p5(std::ifstream & inputfile, std::ofstream & outputfile)
{
    std::string line;
    getline (inputfile,line);
    int linesize = line.size();
    std::vector<bool*> map;
    getBoolRow(map,line,linesize);
    getBoolMap(inputfile,map,linesize);
    int count ;
    int mapsize = map.size();
    for(int i =0 ; i< mapsize ; i++)
    {
        for (int j =0 ; j < linesize ; j++ ) 
        {
            if (sink(map,i,j,mapsize,linesize)) 
            {
                count++;
            }
        }
    }
    std::cout << count;
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
        task1p5(inputfile,outputfile);
        inputfile.close();
        outputfile.close();
    } else {
        std::cout << "fail" << std::endl;
    }
    
    return 0;
}

