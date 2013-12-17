#include "small_buffer.h"

int main() {
    SmallBuffer::read_to_buff_and_save_stats(
            SOURCE_DIR "/tests/input_252.txt", 
            SOURCE_DIR "/tests/output_252.txt");
    return 0;
}

