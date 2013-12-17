/*
 * Search algorithm in standard library (libstdc++-v3 4.8.1) implemented a
 * trivial way therefore may work not fast
 *
 * http://gcc.gnu.org/viewcvs/gcc/tags/gcc_4_8_1_release/libstdc++-v3/include/bits/basic_string.tcc?view=markup
 * 
 * This implementation used the Knuth-Morris-Pratt algorithm 
 * (was taken from "Introduction to Algorithms - Thomas H. Corman and other")
 *
 * */
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#define EXPECT_INPUT_LEN 100000

static const size_t npos = -1;

void compute_prefix(vector<size_t> &prefix, const string &needle){
    const size_t m = needle.length() - 1;
    size_t k = npos;
    prefix.push_back(k);
    for(size_t q=1; q <= m; q++){
        while(k < npos && needle[m - (k + 1)] != needle[m - q]){
            k = prefix[k]; 
        }
        if(needle[m - (k + 1)] == needle[m - q]){
            k++;
        }
        prefix.push_back(k);
    }
}

size_t kmp_matcher(const string &haystack, const string &needle){
    if(haystack.empty() || needle.empty()){
        return npos;
    }
    const size_t n = haystack.length();
    const size_t m = needle.length() - 1;
    vector<size_t> prefix;
    prefix.reserve(m + 1);

    compute_prefix(prefix, needle);

    size_t q = npos;
    for(size_t i=0; i < n; i++){
        while(q < npos && needle[m - (q + 1)] != haystack[i]){
            q = prefix[q];
        }
        if(needle[m - (q + 1)] == haystack[i]){
            q++;
        }
        if(q == m){
            return i - m;
        }
    }
    return npos;
}


void read_line(ifstream &i_fs, string &line){
    if(!line.empty()){
        line.erase();
    }
    int c = '\0';
    c = i_fs.get();
    while(!i_fs.eof()  && c != '\n'){
        if(c != ' ' && c != '-' && c != '\\'){
            line.push_back(static_cast<char>(toupper(c)));
        }
        c = i_fs.get();
    }
}

int main()
{
    ifstream i_fs(SOURCE_DIR "/Input.txt");
    if(!i_fs){
        cout << "Could not open input file" << endl;
        return 1;
    }
    ofstream o_fs(SOURCE_DIR "/Output.txt");
    if(!o_fs){
        cout << "Could not open output file" << endl;
        return 1;
    }
    string text;
    string line;
    size_t offset;
    text.reserve(EXPECT_INPUT_LEN);

    read_line(i_fs, text);
    
    while(!i_fs.eof()){
        read_line(i_fs, line);
        offset = kmp_matcher(text, line);
        if(offset != npos){
            o_fs << "YES" << endl;
        }else{
            o_fs << "NO" << endl;
        }
    }
    i_fs.close();
    o_fs.close();

    return 0;
}
