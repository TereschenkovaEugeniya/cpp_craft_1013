
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/noncopyable.hpp>
#include <boost/integer.hpp>

#include <fstream>


namespace binary_reader {

    class MarketMessage: virtual protected boost::noncopyable
    {
    public:
        MarketMessage(){}
        ~MarketMessage(){}

        static const int message_size = 8;
        static const int date_size = 8;

        void read( std::fstream& );
        void write( std::fstream& );

    private:
        char                    stock_name_ [ message_size + 1 ];
        boost::gregorian::date  date_time_;
        double                  price_;
        double                  vwap_;
        boost::uint32_t         volume_;
        double                  f1;
        double                  t1;
        double                  f2;
        double                  f3;
        double                  f4;
    };

} // namespace binary_reader
