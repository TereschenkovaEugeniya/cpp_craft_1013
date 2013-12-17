#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <boost/noncopyable.hpp>
#include <boost/integer.hpp>

#include <vector>
#include <fstream>

namespace binary_reader{
    class Message: virtual protected boost::noncopyable
    {

    public:
        Message(){}
        ~Message();
        void write(std::fstream& out );
        void read(std::fstream& in);

        boost::uint32_t getTime () const;
        boost::uint32_t getType () const;
        boost::uint32_t size() const;

    private:
        boost::uint32_t type_;
        boost::uint32_t time_;
        boost::uint32_t len_;

        char* msg_;
    };


} // binary_reader




#endif
