#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include <boost/cstdint.hpp>

namespace binary_reader {
    class Reader {
        public:
            Reader(const std::string filename);
            template <typename T> const T get_binary()
            {
                T t;
                in.read(reinterpret_cast<char *>(&t), sizeof( T ) );
                return t;
            } 
            bool is_open() const;
            bool eof() const;
            std::string get_string(size_t);
            ~Reader();
        private:
            static const size_t STR_BUF_SIZE = 102400;
            std::ifstream in;
            Reader();
            Reader(const Reader&);
            void operator=(const Reader&);
    }; 
}
