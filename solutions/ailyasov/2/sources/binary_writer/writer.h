#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

namespace binary_writer
{
    class Writer {
        public:
            Writer(const std::string filename);
            template <typename T> void save_binary( const T & v)
            {
                out.write(reinterpret_cast<const char*>(&v), sizeof( T ) );
            }
            bool eof() const;
            bool is_open() const;
            void save_string(const std::string&);
            ~Writer();
        private:
            std::ofstream out;
            Writer();
            Writer(const Writer&);
            void operator=(const Writer&);
    };
}
