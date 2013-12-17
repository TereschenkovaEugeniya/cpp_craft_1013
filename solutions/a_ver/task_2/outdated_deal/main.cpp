#include <stdint.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <deal.h>

using namespace std;

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
    MessageHeader m_header;

    while(i_fs.good()){
        i_fs >> m_header;

        if(i_fs.fail()){
            break;
        }
        if(is_outdated_deal(m_header)){
            i_fs.seekg(m_header.len, ios_base::cur);
            continue;
        }
        o_fs.write((char *)&m_header, sizeof(MessageHeader));
        copy_n(istreambuf_iterator<char>(i_fs), m_header.len,
               ostreambuf_iterator<char>(o_fs));
        i_fs.seekg(1, ios_base::cur);
    }
    i_fs.close();
    o_fs.close();
    return 0;
}
