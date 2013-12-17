/*
 * Realization with sorting
 *
 * realization for case when all messages could be loaded to memory
 */
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <deal.h>

using namespace std;

struct ExtMessageHeader: MessageHeader{
    uint64_t offset;
};

const static unsigned short int BUFFER_SIZE = -1; // 65535
const static unsigned short int COPY_BUFFER_SIZE = 255;

bool comparator(const ExtMessageHeader &item1, const ExtMessageHeader &item2){
    return item1.time < item2.time;
}

int main(){
    ifstream i_fs(SOURCE_DIR "/Input.in", ifstream::binary);
    ofstream o_fs(SOURCE_DIR "/Output.bin", ofstream::binary);
    if(!i_fs){
        cout << "Could not open input file" << endl;
        return 1;
    }
    if(!o_fs){
        cout << "Could not open output file" << endl;
        return 1;
    }
    vector<ExtMessageHeader> buffer;
    buffer.reserve(BUFFER_SIZE);

    vector<ExtMessageHeader>::iterator it = buffer.begin();

    buffer.resize(buffer.size()+1);
    while(i_fs.good() && buffer.size() < BUFFER_SIZE){
        i_fs >> *it;
        (*it).offset = i_fs.tellg();
        i_fs.seekg(it->len, ios_base::cur);

        if(is_outdated_deal(*it)){
            continue;
        }
        it++;
        buffer.resize(buffer.size()+1);
    }
    buffer.resize(buffer.size()-1);

    sort(buffer.begin(), buffer.end(), comparator);

    i_fs.clear();

    for(vector<ExtMessageHeader>::iterator it = buffer.begin(); it < buffer.end(); it ++){
        o_fs << *it;
        i_fs.seekg(it->offset);
        copy_n(istreambuf_iterator<char>(i_fs), it->len,
               ostreambuf_iterator<char>(o_fs));
        i_fs.seekg(1, ios_base::cur);
    }
    i_fs.close();
    o_fs.close();

    i_fs.open(SOURCE_DIR "/Output.bin", ifstream::binary);
    dump_to_stream(i_fs, cout);
    i_fs.close();

    return 0;
}