/*
 *       Filename:  task15.cpp
 *    Description:  Solution for task # 15 Islands
 */ 

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include "task15.h"

static const std::string TEST_DIR(SOURCE_DIR "/tests/");
static const std::string OUTPUT_FILE (TEST_DIR + "output.txt");
static const std::string INPUT_FILE (TEST_DIR + "input3.txt");

int main(int argc, char **argv)
{
    std::ofstream out(OUTPUT_FILE.c_str());
    if(!out.good()) {
        std::cout << "Error opening ouput file " << OUTPUT_FILE << std::endl;
        exit(1);
    }

    int total = 0;

    Land land(INPUT_FILE);

    for(int i = 0; i < land.heigth(); i++) 
        for(int j = 0; j < land.width(); j++) 
            if(land.is_set(i, j)) {
                land.walk_and_mark(i, j);
                ++total;
            }

    out << total << std::endl;
    std::cout << "Output file: " << OUTPUT_FILE << std::endl;
    return 0;
}
