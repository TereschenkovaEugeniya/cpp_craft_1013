#include <iterator>
#include <cstddef>
#include <vector>
#include <stdexcept>
#include "writer.h"

namespace binary_writer {

    Writer::Writer(const std::string filename) {
        out.open(filename.c_str(), std::ios::out | std::ios::binary);
#ifndef NDEBUG
        std::cout << "Output: " << filename << std::endl;
#endif
    }

    void Writer::save_string(const std::string& s) {
        out.write(s.c_str(), s.size());
    }

    bool Writer::is_open() const {
        return out.is_open();
    }

    bool Writer::eof() const {
        return out.eof();
    }

    Writer::~Writer() {
        out.close();
    }

}
