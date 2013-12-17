#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <exception>

/*
* Alexandr Lapenkov
* Cpp_craft, task 1.5
*/

using namespace std;

class Solution
{

private:
         
    int n,m;
    vector<string>a;
         
    ifstream in;
    ofstream out;

	struct point
	{
		int x,y;

		point()
		{
			x=0;
			y=0;
		}

		point(int x, int y)
		{
			point::x=x;
			point::y=y;
		}

		point up(){
			return point(x-1,y);
		}

		point left(){
			return point(x,y-1);
		}

		point right(){
			return point(x,y+1);
		}

		point down(){
			return point(x+1,y);
		}

	};

public:
       
       
    Solution()
    {
        in.open(SOURCE_DIR"/input.txt",ios_base::in);
        out.open(SOURCE_DIR"/output.txt",ios_base::out);
        
        if(!in.is_open()||!out.is_open())
            throw new exception();
    }
            
    ~Solution()
    {
        in.close();
        out.close();
    }
       
    inline void bfs_move(queue<point>&q, const point& to)
    {
    
        if(to.x<0||to.y<0||to.x>=n||to.y>=m)return ;
        if(a[to.x][to.y]!='o')return ;
        
        q.push(to);
          
        a[to.x][to.y]='~';
          
    }  


    void bfs_component(const point& start)
    {
        queue<point>q;
        point current;
 
        q.push(start);
    
        a[start.x][start.y]='~';
    
        while(!q.empty())
        {
            current = q.front();
            q.pop();
           
            bfs_move(q,current.up());
            bfs_move(q,current.down());
            bfs_move(q,current.left());
            bfs_move(q,current.right());
        }

    }
            
    void process()
    {     
        string buf;
        int ans=0;
       
        while(!in.eof())
        {
            if(!(in>>buf))break;
            a.push_back(buf);   
        }

        n=(int)a.size();
        m=(int)a[0].size();
          
        for(int i=0;i<n;i++)
            for(int j=0;j<m;j++)
                if(a[i][j]=='o')
                {
                    ans++;
                    bfs_component(point(i,j));
                }
        
        out<<ans;
                 
    }

};


int main()
{
       
    try
    {
        Solution s;
        s.process();
    } catch (...)
    {
        cerr<<"Can't open files!";
    }

    return 0;  
}

