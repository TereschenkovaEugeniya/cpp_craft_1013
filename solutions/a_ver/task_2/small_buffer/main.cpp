#include <stdint.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include <deal.h>

using namespace std;

struct Aggregated{
    uint32_t count;
    uint32_t count_seconds;
    uint32_t current_time;
    uint32_t current_buffer_size;

    Aggregated() : count(0), count_seconds(0), current_time(0), current_buffer_size(0) {}
};

const static unsigned short int MAX_BUFFER_SIZE = 2048;
const static unsigned int MAX_THRESHOLD_TYPE = 100000;

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
    map<uint32_t, Aggregated> aggregated_data;

    while(i_fs.good()){
        i_fs >> m_header;
        if(i_fs.fail()){
            break;
        }
        i_fs.seekg(m_header.len, ios_base::cur);

        if(m_header.time > MAX_THRESHOLD_TYPE){
            continue;
        }

        if(aggregated_data.find(m_header.type) == aggregated_data.end() ){
            aggregated_data[m_header.type] = Aggregated();
        }
        
        Aggregated &item = aggregated_data[m_header.type];

        if(item.current_time != m_header.time){
            item.current_time = m_header.time;
            item.count_seconds++;
            item.current_buffer_size = 0;
        }
        if( (item.current_buffer_size + m_header.len + M_HEADER_SIZEOF) > MAX_BUFFER_SIZE){
            continue;
        }
        item.current_buffer_size += m_header.len;
        item.count += 1;
    }

    double average_count;
    for (auto it=aggregated_data.cbegin(); it!=aggregated_data.cend(); ++it){
        o_fs.write(reinterpret_cast<const char*>(&it->first), sizeof(it->first));
        average_count = (it->second.count / (double)it->second.count_seconds);
        o_fs.write(reinterpret_cast<const char*>(&average_count), sizeof(average_count));
    }

    i_fs.close();
    o_fs.close();
    return 0;
}
