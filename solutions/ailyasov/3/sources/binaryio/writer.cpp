#include <iterator>
#include <cstddef>
#include <vector>
#include <stdexcept>

#include <boost/thread/thread.hpp>

#include "writer.h"

namespace binaryio {

    Writer::Writer(const std::string filename) {
        out.open(filename.c_str(), std::ios::out | std::ios::binary);
#ifndef NDEBUG
        boost::mutex::scoped_lock lock ( cout_protector_ );
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
