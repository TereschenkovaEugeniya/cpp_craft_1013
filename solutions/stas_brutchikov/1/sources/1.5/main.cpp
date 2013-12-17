#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm> 
#include <stdint.h> 

const char c_land = 'o';
const char c_water = '~';
struct Point
{
    int32_t i;
    int32_t j;
};
typedef std::vector<std::string> Field;


void SinkIsland(Field& field, const Point& point)
{
    std::queue<Point> q;
    q.push(point);
    while(!q.empty())
    {
        Point& curr = q.front();
        field[curr.i][curr.j] = c_water;
        for(auto i = -1; i <=1; ++i)
            for(auto j = -1; j <=1; ++j)
                if (i*i^j*j)
                {
                    Point n = {curr.i + i, curr.j + j};
                    if (n.i >= 0 
                        && n.i < (int32_t)field.size()
                        && n.j >= 0
                        && n.j < (int32_t)field[n.i].size()
                        && field[n.i][n.j] == c_land
                        )
                        q.push(n);                               
                }
        q.pop();
    }    
}


int32_t SinkIslands(Field& field)
{
    int32_t res= 0;
    for(int32_t i = 0; i < (int32_t)field.size(); ++i)
        for(int32_t j = 0; j < (int32_t)field[i].size(); ++j)
        {
            if (field[i][j] == c_land)
            {
                Point s = {i,j};
                SinkIsland(field, s);
                ++res;
            }
        }
    return res;
};


int main()
{
    setlocale(LC_ALL, "");
    std::ifstream in (BINARY_DIR "/Input.txt");
    std::ofstream out (BINARY_DIR "/Output.txt");

	if (!in.is_open()|| !out.is_open())
    {
        return 1;
    }

    Field field;
    std::string line;
    while(!in.eof())
    {
        std::getline(in, line);
        field.push_back(line);
    };

    out << SinkIslands(field) <<std::endl;       
}

