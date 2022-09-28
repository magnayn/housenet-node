#ifndef __ESP8266_PREFERENCES__
#define __ESP8266_PREFERENCES__
#ifdef ESP8266

#include <Arduino.h>
#include <ArduinoJson.h>

class Preferences
{
    public:
        Preferences();

        void begin(String a, bool b);
        String getString(String key, String def);
        void putString(String key, String value);
        void end();
    
    private:
        void save();
        void debug();

        DynamicJsonDocument doc;
        JsonObject root;
        JsonObject object;
};




#endif
#endif