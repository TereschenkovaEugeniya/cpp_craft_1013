//#include <cstdint>
#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include <array>
#include <time.h> 

using namespace std;

struct Message{
    char stock_name[8];
    char date_time[8];
    double price;
    double vwap;
    uint32_t volume;
    double f1;
    double t1;
    double f2;
    double f3;
    double f4;
};

int main(){
    ifstream i_fs(SOURCE_DIR "/input.txt", ifstream::binary);
    ofstream o_fs(SOURCE_DIR "/output.txt", ofstream::binary);
    if(!i_fs){
        cout << "Could not open input file" << endl;
        return 1;
    }
    if(!o_fs){
        cout << "Could not open output file" << endl;
        return 1;
    }
    Message message;
    short unsigned int year, month, day;
    uint32_t count_days;

    while(i_fs.good()){
        fill((char *)&message, (char *)&message + sizeof(Message), 0);

        i_fs.read((char *)&message.stock_name, sizeof(message.stock_name));
        i_fs.read((char *)&message.date_time, sizeof(message.date_time));
        i_fs.read((char *)&message.price, sizeof(message.price));
        i_fs.read((char *)&message.vwap, sizeof(message.vwap));
        i_fs.read((char *)&message.volume, sizeof(message.volume));
        
        i_fs.read((char *)&message.volume, sizeof(message.f1));
        i_fs.read((char *)&message.t1, sizeof(message.t1));
        i_fs.read((char *)&message.f2, sizeof(message.f2));
        i_fs.read((char *)&message.f3, sizeof(message.f3));
        i_fs.read((char *)&message.f4, sizeof(message.f4));
        if(i_fs.fail()){
            break;
        }
        
        sscanf(message.date_time, "%4hu%2hu%2hu", &year, &month, &day);

        count_days = 372*(year-1) + 31*(month-1) + day;

        o_fs.write((char *)&message.stock_name, 8);
        o_fs << '\0';
        o_fs.write((char *)&count_days, sizeof(count_days));
        o_fs.write((char *)&message.vwap, sizeof(message.vwap));
        o_fs.write((char *)&message.volume, sizeof(message.volume));
        o_fs.write((char *)&message.f2, sizeof(message.f2));
    }

    i_fs.close();
    o_fs.close();

    return 0;
}
