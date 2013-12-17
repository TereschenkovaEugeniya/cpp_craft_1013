#include "market_message.h"

void binary_reader::MarketMessage::read(std::fstream &stream )
{
    char date[ date_size + 1 ];

    stream.read( reinterpret_cast<char*> (&stock_name_), sizeof(char) * message_size );
    stream.read( reinterpret_cast<char*> (&date), sizeof(char) * date_size );
    stream.read( reinterpret_cast<char*> (&price_), sizeof(price_) );
    stream.read( reinterpret_cast<char*> (&vwap_), sizeof(vwap_) );
    stream.read( reinterpret_cast<char*> (&volume_), sizeof(volume_) );
    stream.read( reinterpret_cast<char*> (&f1), sizeof(f1) );
    stream.read( reinterpret_cast<char*> (&t1), sizeof(t1) );
    stream.read( reinterpret_cast<char*> (&f2), sizeof(f2) );
    stream.read( reinterpret_cast<char*> (&f3), sizeof(f3) );
    stream.read( reinterpret_cast<char*> (&f4), sizeof(f4) );

    if ( ! stream.eof() )
        date_time_ = boost::gregorian::from_undelimited_string( date );

}

void binary_reader::MarketMessage::write(std::fstream &stream)
{
    boost::gregorian::date startDate = boost::gregorian::from_undelimited_string( "14010101");
    boost::gregorian::date_duration days = date_time_ - startDate;

    long days_num = days.days() + ( 1400 * 372 );


    stream.write( reinterpret_cast<char*> (&stock_name_), sizeof(char) * message_size );
    stream.write( reinterpret_cast<char*> (&days_num), sizeof( days_num ));
    stream.write( reinterpret_cast<char*> (&vwap_), sizeof(vwap_) );
    stream.write( reinterpret_cast<char*> (&volume_), sizeof(volume_) );
    stream.write( reinterpret_cast<char*> (&f2), sizeof(f2) );

}
