/*
 * Realization with sorting
 *
 * Used two buffers: one for performing quick sort of partial data,
 * second for performing merge sort data obtained in step of quick sort.
 *
 * 1. Read data to buffer for quick sort
 * 2. When buffer is full - perform quick sort. Then flushing data from
 *    buffer into temporary file and remember offset
 * 3. After reading all data from input file perform merge sort with
 *    copy data from original input to result output file
 */
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <queue>
#include <iterator>
#include <deal.h>

using namespace std;

typedef unsigned short int chunk_size_t;
//const chunk_size_t QSORT_BUFFER_SIZE = -1; // 65535
const chunk_size_t QSORT_BUFFER_SIZE = 5;

struct ExtMessageHeader: MessageHeader{
    uint64_t offset;
};

const uint32_t SIZEOF_EXT = sizeof(uint32_t)*3 + sizeof(uint64_t);


istream& operator>> (istream& is, ExtMessageHeader &m){
    is >> static_cast<MessageHeader &>(m);
    is.read((char *)&m.offset, sizeof(m.offset));
    return is;
}


ostream& operator<< (ostream& os, const ExtMessageHeader &m){
    os << static_cast<const MessageHeader &>(m);
    os.write((char *)&m.offset, sizeof(m.offset));
    return os;
}


struct Chunk{
    uint64_t offset;
    chunk_size_t count;

    Chunk(): offset(0), count(0){}
    Chunk(uint64_t arg_offset, chunk_size_t arg_count): offset(arg_offset), count(arg_count){}
};


bool comparator(const ExtMessageHeader &item1, const ExtMessageHeader &item2){
    return item1.time < item2.time;
}


void flush_buffer(vector<ExtMessageHeader> &qsort_buffer, fstream &t_fs, queue<Chunk> &msort_queue){
    msort_queue.push(Chunk(t_fs.tellp(), (chunk_size_t) qsort_buffer.size()));

    sort(qsort_buffer.begin(), qsort_buffer.end(), comparator);

    for(vector<ExtMessageHeader>::iterator it = qsort_buffer.begin(); it < qsort_buffer.end(); it++){
        t_fs << (*it);
    }
}


void merge_by_one(fstream &from_fs, fstream &to_fs, Chunk &first, Chunk &second){
    ExtMessageHeader item_first, item_second;
    chunk_size_t first_number = 0;
    chunk_size_t second_number = 0;

    from_fs.clear();
    to_fs.clear();

    from_fs.seekg(first.offset);
    from_fs >> item_first;

    from_fs.seekg(second.offset);
    from_fs >> item_second;

    while(first_number < first.count && second_number < second.count){
        if(comparator(item_first, item_second)){
            to_fs << item_first;

            first_number++;
            from_fs.seekg(first.offset + first_number * SIZEOF_EXT);
            from_fs >> item_first;
        }else{
            to_fs << item_second;

            second_number++;
            from_fs.seekg(second.offset + second_number * SIZEOF_EXT);
            from_fs >> item_second;
        }
    }

    if(first_number > 0){
        from_fs.seekg(first.offset + first_number * SIZEOF_EXT);
        copy_n(istreambuf_iterator<char>(from_fs),
               (first.count - first_number) * SIZEOF_EXT,
               ostreambuf_iterator<char>(to_fs));
    }
    if(second_number > 0){
        from_fs.seekg(second.offset + second_number * SIZEOF_EXT);
        copy_n(istreambuf_iterator<char>(from_fs),
               (second.count - second_number) * SIZEOF_EXT,
               ostreambuf_iterator<char>(to_fs));
    }
}
/*
void merge_bulk(fstream &i_fs, fstream &o_fs, Chunk &first, Chunk &second){
    // to read with buffering more than one elements
}
*/


void merge_sort(fstream &from_fs, queue<Chunk> &msort_queue){
    size_t size = msort_queue.size();
    if(size < 2){
        return;
    }

    fstream to_fs(SOURCE_DIR "/Temporary2.bin",
                  fstream::binary | fstream::in | fstream::out | fstream::trunc);

    if(!to_fs){
        cout << "Could not open swap file" << endl;
        return;
    }

    fstream *from_fs_p = &from_fs;
    fstream *to_fs_p = &to_fs;

    while(msort_queue.size() > 1){
        Chunk &first = msort_queue.front();
        msort_queue.pop();
        Chunk &second = msort_queue.front();
        msort_queue.pop();

        merge_by_one(*from_fs_p, *to_fs_p, first, second);
        size -= 2;
        msort_queue.push(
            Chunk(first.offset, (chunk_size_t) (first.count + second.count)));

        if(size == 1){
            Chunk &first = msort_queue.front();
            msort_queue.pop();

            copy_n(istreambuf_iterator<char>(from_fs),
                   first.count * SIZEOF_EXT,
                   ostreambuf_iterator<char>(to_fs));

            msort_queue.push(first);
        }
        if(size == 0){
            size = msort_queue.size();
            swap(from_fs_p, to_fs_p);
        }
    }
    if(from_fs_p == &from_fs){
        to_fs.close();
    }else{
        to_fs.close();
        from_fs.close();
        from_fs.open(SOURCE_DIR "/Temporary2.bin",
                   fstream::binary | fstream::in | fstream::out);
    }
}


int main(){
    ifstream i_fs(SOURCE_DIR "/Input.in", ifstream::binary);
    ofstream o_fs(SOURCE_DIR "/Output.bin", ofstream::binary);
    fstream t_fs(SOURCE_DIR "/Temporary.bin",
                 fstream::binary | fstream::out | fstream::in | fstream::trunc);

    if(!i_fs){
        cout << "Could not open input file" << endl;
        return 1;
    }
    if(!o_fs){
        cout << "Could not open output file" << endl;
        return 1;
    }
    if(!t_fs){
        cout << "Could not open temporary file" << endl;
        return 1;
    }

    queue<Chunk> msort_queue;
    vector<ExtMessageHeader> qsort_buffer;
    vector<ExtMessageHeader>::iterator it;
    qsort_buffer.reserve(QSORT_BUFFER_SIZE);
    qsort_buffer.resize(1);
    it = qsort_buffer.begin();

    while(i_fs.good()){
        i_fs >> static_cast<MessageHeader &>(*it);
        if(i_fs.fail()){
            break;
        }
        it->offset = i_fs.tellg();

        // pass data
        i_fs.seekg(it->len, ios_base::cur);

        if(is_outdated_deal(*it)){
            continue;
        }

        if(qsort_buffer.capacity() == qsort_buffer.size()){
            cout << "flush buffer" << endl;
            flush_buffer(qsort_buffer, t_fs, msort_queue);
            qsort_buffer.resize(1);
            it = qsort_buffer.begin();
            continue;
        }
        it++;
        qsort_buffer.resize(qsort_buffer.size()+1);
    }
    if(qsort_buffer.size() > 0){
        qsort_buffer.resize(qsort_buffer.size()-1);
        flush_buffer(qsort_buffer, t_fs, msort_queue);
    }

    merge_sort(t_fs, msort_queue);

    t_fs.clear();
    i_fs.clear();

    ExtMessageHeader ext_m;

    t_fs.seekg(0);
    while(t_fs.good()){
        t_fs >> ext_m;
        if(t_fs.fail()){
            break;
        }
        i_fs.seekg(ext_m.offset);
        o_fs << static_cast<const MessageHeader>(ext_m);
        copy_n(istreambuf_iterator<char>(i_fs),
               ext_m.len,
               ostreambuf_iterator<char>(o_fs));
    }

    i_fs.close();
    t_fs.close();
    o_fs.close();

    i_fs.open(SOURCE_DIR "/Output.bin", ifstream::binary);
    dump_to_stream(i_fs, cout);
    i_fs.close();
    return 0;
}