#include <stdint.h>
#include <iostream>
#include <deal.h>
#include <sstream>
#include <algorithm>

using namespace std;

istream& operator>> (istream& is, MessageHeader &m){

    is.read((char *)&m.type, sizeof(m.type));
    is.read((char *)&m.time, sizeof(m.time));
    is.read((char *)&m.len, sizeof(m.len));

    return is;
}

ostream& operator<< (ostream& os, const MessageHeader &m){
    os.write((char *)&m.type, sizeof(m.type));
    os.write((char *)&m.time, sizeof(m.time));
    os.write((char *)&m.len, sizeof(m.len));

    return os;
}

bool is_outdated_deal(MessageHeader &item){
    static uint32_t max_time = 0;
    bool result = true;
    if((item.type == MARKET_OPEN || item.type == TRADE ||
        item.type == QUOTE || item.type == MARKET_CLOSE) &&
       (item.time > (max_time - 2) || max_time < 2)){
        result = false;
    }
    if(max_time < item.time){
        max_time = item.time;
    }
    return result;
}

string MessageHeader::as_string(){
    stringstream buffer;
    buffer << "type = " << this->type;
    buffer << "; time = " << this->time;
    buffer << "; len = " << this->len;
    return  buffer.str();
}

void dump_to_stream(istream& is, ostream& os){
    MessageHeader m;

    while(is.good()){
        is >> m;
        if(is.fail()){
            break;
        }
        os << m.as_string();
        os << "; data: ";
        copy_n(istreambuf_iterator<char>(is), m.len,
               ostreambuf_iterator<char>(cout));
        is.seekg(1, ios_base::cur);
        os << endl;
    }
}