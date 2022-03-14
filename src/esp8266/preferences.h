#ifndef __ESP8266_PREFERENCES__
#define __ESP8266_PREFERENCES__
#ifdef ESP8266

#include <Arduino.h>

class Preferences
{
    public:
        void begin(String a, bool b);
        String getString(String key, String def);
        void putString(String key, String value);
        void end();
};




#endif
#endif