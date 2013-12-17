#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <utility>
#include <string>
#include <algorithm>

using namespace std;


void mark_island(vector<string>& data, size_t i, size_t j){
    stack<pair<size_t, size_t> > st;
    pair<size_t, size_t> p;
    const size_t column_length = data.size() - 1;
    const size_t row_length = data[0].length() - 1;
    
    st.push(make_pair(i, j));
    while(!st.empty()){
        p = st.top();
        st.pop();
        data[p.first][p.second] = 'x';

        if(p.second > 0 && data[p.first][p.second-1] == 'o'){
            st.push(make_pair(p.first, p.second-1));
        }
        if((p.second + 1) < row_length && data[p.first][p.second+1] == 'o'){
            st.push(make_pair(p.first, p.second+1));
        }
        if(p.first > 0 && data[p.first-1][p.second] == 'o'){
            st.push(make_pair(p.first-1, p.second));
        }
        if((p.first + 1) < column_length && data[p.first+1][p.second] == 'o'){
            st.push(make_pair(p.first+1, p.second));
        }
    }
}

int main(){
    ifstream i_fs(SOURCE_DIR "/Input.txt");
    ofstream o_fs(SOURCE_DIR "/Output.txt");
    if(!i_fs){
        cout << "Could not open input file" << endl;
        return 1;
    }
    if(!o_fs){
        cout << "Could not open output file" << endl;
        return 1;
    }
    vector<string> data;
    string line;
    size_t count = 0;

    while(!i_fs.eof()){
        i_fs >> line;
        data.push_back(line);
    }
    const size_t column_length = data.size();
    const size_t row_length = data[0].length();

    for(size_t i=0; i < column_length; i++){
        for(size_t j=0; j < row_length; j++){
            if(data[i][j] == 'o'){
                mark_island(data, i, j);
                count++;
            }
        }
    }
    o_fs << count;
    o_fs.close();
    i_fs.close();
    return 0;
}
