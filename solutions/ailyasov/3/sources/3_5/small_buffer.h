#pragma once 

#include <string>

class SmallBuffer {
    public:
        static void read_to_buff_and_save_stats(const std::string& input, const std::string& output);
    private:
        SmallBuffer();
        SmallBuffer(const SmallBuffer&);
        void operator=(const SmallBuffer&);
};
