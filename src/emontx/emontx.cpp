//#include <Arduino.h>
#include "emontx.h"
#include "debugging.h"

#define RXD2 16
#define TXD2 17

#ifdef ESP32
  #define SERIAL_PORT Serial2
#else
  #define SERIAL_PORT Serial1
#endif


/// TODO: EmonTX::EmonTX() : serialPort(Serial2)
EmonTX::EmonTX() : serialPort(SERIAL_PORT)
{
	Serial.println("Make EmonTX");

	_function = nullptr;

#ifdef ESP32
  serialPort.begin(115200,SERIAL_8N1,RXD2, TXD2);
#else
  serialPort.begin(115200,SERIAL_8N1);
#endif

	
	serialPort.setTimeout(0);
}

void EmonTX::process()
{
	
	while (serialPort.available())
	{
		Serial.println("Serial Process");

		String str = serialPort.readString();
		Serial.printf("Read: %s\n", str.c_str());
		//node->debug(str);

		_message += str;
		//node->debug(_message);
		
		Serial.printf("Buff: %s\n", _message.c_str());
		processBuffer();

		Serial.println("Serial Process Done");

		// Lets not go on forever with this.
		if( _message.length() > 1024) {
			_message = "";
		}
		//TODO: ESP8266 only?
		//ESP.wdtFeed();
		//
	}
}

void EmonTX::processBuffer() {
	int idx;
	while( (idx = _message.indexOf('\n')) > 0 )	
	{
		String msg = _message.substring(0, idx-1);

		processMessage(msg);

		unsigned int next = idx + 1;
		if (next > _message.length())
			_message = "";
		else
			_message = _message.substring(next);
	}
}


void EmonTX::onMessage(EmonTXHandlerFunction onRequest)
{
	this->_function = onRequest;
}



void EmonTX::processMessage(const String &message)
{
	Debugging.print("Processing [");
	Debugging.print(message);
	Debugging.println("]");

	// data we're reading
	EmonMessage emm;
	if (emm.parse(message) && _function != NULL)
	{
		Debugging.println("CallFunc");
		_function(emm);
		Debugging.println("RetOK");
	}
}