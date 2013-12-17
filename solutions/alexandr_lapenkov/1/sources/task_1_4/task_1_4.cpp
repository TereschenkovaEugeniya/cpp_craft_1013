#include <iostream>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <vector>

/*
* Alexandr Lapenkov
* Cpp_craft, task 1.4
*/

using namespace std;

class Solution{

    static const int EPS=1;

    int n;
    long double x;
    vector<long double>a;
     
    ifstream in;
    ofstream out;
   
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
               
            
    void process()
    {
        
        in>>n;
        a.reserve(n);
                 
        for(int i=0;i<n;i++)
        {
            in>>x;
            a.push_back(floor(x*10000));
        }
              
        sort(a.begin(),a.end());
              
        while(!in.eof())
        {
            vector<long double>::iterator it;
                            
            if(!(in>>x))
                break;

            x=floor(x*10000);
                            
            it = lower_bound(a.begin(),a.end(),x);
            
            if(it!=a.end() && fabs((*it)-x)<EPS)
                out<<"YES\n";
            else
               if(it!=a.begin())
               {
                   it--;
                                                    
                   if(fabs((*it)-x)<EPS)
                       out<<"YES\n";
                   else 
                       out<<"NO\n";
               }
               else 
                   out<<"NO\n";
                               
        }   
      
    }
   
};

int main()
{
       
    try
    {
        Solution s;
        s.process();
    }catch(...)
    {
        cerr<<"Can't open files!";
    }
           
    return 0;  
}
