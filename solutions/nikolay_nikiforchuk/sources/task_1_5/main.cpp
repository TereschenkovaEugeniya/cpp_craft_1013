#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <set>

using namespace std;

void logLine(vector<int*> &line){
    std::cout << "Current line" << std::endl;
    std::vector<int*>::iterator it = line.begin();
    for(it; it != line.end(); ++it){
        std::cout << "-" << (*((int*)*it));
     }
    std::cout << endl;
}

class Walker{
    vector<int*> landIds;
    vector<int*> pointsBuffer;
    string line;

    public:
        static int WATER_ID;
        static int landUid;
        int walk(std::ifstream& input){
            while(!input.eof()){
                std::getline(input, line);
                std::string::iterator it = line.begin();
                if(pointsBuffer.size() == 0){
                    pointsBuffer.resize(line.size(), &WATER_ID);
                }
                for(it; it != line.end(); ++it){
                    int position = std::distance(line.begin(), it);
                    if(!isWater(*it)){
                        int* id = getLandId(position);
                        pointsBuffer[position] = id;
                    } else {
                        pointsBuffer[position] = &WATER_ID;
                    }
                }
                //logLine(pointsBuffer);
            }
            //logLine(landIds);
            std::set<int*> landsUniq( landIds.begin(), landIds.end());
            return landsUniq.size();
        }
        int* getLandId(const int& position){
            int* upLandId = getUpLandId(position);
            int* leftLandId = getLeftLandId(position);
            if (*upLandId == *leftLandId){
                if(*upLandId != WATER_ID){
                    return upLandId;
                } else {
                    int* id = new int(landUid++);
                    landIds.push_back(id);
                    return id;
                }
            }
            if((*upLandId == WATER_ID) && (*leftLandId != WATER_ID)){
                return leftLandId;
            }
            if((*leftLandId == WATER_ID) && (*upLandId != WATER_ID)){
                return upLandId;
            }
            if (*upLandId != *leftLandId){
                return mergeLands(*upLandId, *leftLandId);
            }
            return &WATER_ID;
        }

        int* getUpLandId(const int position){
            return pointsBuffer.at(position);
        }

        int* getLeftLandId(const int position){
            if(position > 0){
                return pointsBuffer.at(position - 1);
            }
            return &WATER_ID;
        }

        int* mergeLands(int& land1, int& land2){
            int* id;
            if(*(&land1) > *(&land2)){
                landIds[*(&land1)] = &land2;
                land1 = land2;
                id = &land2;
            } else {
                landIds[*(&land2)] = &land1;
                land2 = land1;
                id = &land1;
            }
            return id;
        }

        bool isWater(char symbol){
            if(symbol == '~'){
                return true;
            }
            return false;
        }
        ~Walker(){
            for ( int i = 0; i < pointsBuffer.size(); i++ ){
                delete pointsBuffer[i];
            }
            pointsBuffer.clear();
            for ( int i = 0; i < landIds.size(); i++ ){
                delete landIds[i];
            }
            landIds.clear();
        };
};



int Walker::landUid = 0;
int Walker::WATER_ID = -1;

int main()
{
    int count;
    Walker walker;
    std::ifstream input("input.txt");
    if(input.good()){
       count = walker.walk(input);
    } else {
        std::cout << "Can't open file" << std::endl;
    }
    input.close();
    std::fstream resultFile( "output.txt", std::fstream::out | std::fstream::trunc );
    if(!resultFile.is_open()){
        cout << "Can't open file to write a result" << endl;
    }
    resultFile << count << endl;
    resultFile.close();
    return 0;
}
