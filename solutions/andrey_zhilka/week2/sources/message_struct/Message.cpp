#include "Message.h"

Message::~Message() {
	delete [] this->text;
}

void Message::setTime( unsigned time ) {
	this->time = time;
}

void Message::setType( unsigned type ) {
	this->type = type;
}

void Message::setLength( unsigned length ) {
	this->length = length;
}

void Message::setText( char* text ) {
	if ( !this->text || strlen(this->text) < strlen( text ) )
	{
		delete [] this->text;
		this->length = strlen( text );
		this->text = new char[ length+1 ];
	}
	strcpy( this->text, text );
}

unsigned Message::getLength() const {
	return this->length;
}

unsigned Message::getTime() const {
	return this->time;	
}

unsigned Message::getType() const {
	return this->type;
}

bool Message::isOk() const {
	return this->is_ok;
}

std::istream& operator>>( std::istream& binary_is, Message& message ) {
	binary_is.read( reinterpret_cast< char* > ( &message.type ), sizeof( unsigned ) );
	binary_is.read( reinterpret_cast< char* > ( &message.time ), sizeof( unsigned ) );
	binary_is.read( reinterpret_cast< char* > ( &message.length ), sizeof( unsigned ) );
	message.text = new char[ message.length + 1 ];
	binary_is.read( message.text, message.length );
	message.text[ message.length ] = 0;

	message.is_ok = ( message.type >= 1 && message.type <= 4 );

	return binary_is;
}

std::ostream& operator<<( std::ostream& binary_os, const Message& message ) {
	binary_os.write( reinterpret_cast< const char* > ( &message.type ), sizeof( unsigned ) );
	binary_os.write( reinterpret_cast< const char* > ( &message.time ), sizeof( unsigned ) );
	binary_os.write( reinterpret_cast< const char* > ( &message.length ), sizeof( unsigned ) );
	binary_os.write( message.text, message.length );

	return binary_os;
}