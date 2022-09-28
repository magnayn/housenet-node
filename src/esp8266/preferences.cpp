#ifdef ESP8266
#include "preferences.h"
#include <ArduinoJson.h>
#include <LittleFS.h>



Preferences::Preferences() : doc(1024) {
    Serial.print("------ Loading Preferences ----\n");
    File file = LittleFS.open("/Config.txt", "r");

    deserializeJson(doc, file);
    
    file.close();


    debug();
    

    root = doc.as<JsonObject>();
    debug();

    Serial.print("------ Loading Preferences Done ----\n");
}

void Preferences::begin(String a, bool b)
{
    
    Serial.printf("Begin prefs [%s]\n", a.c_str());
    debug();

    if( root.containsKey(a) ) {
        Serial.println("Exists");
        object = root[a];
    } else {
        Serial.println(" Not Exists");
        object = root.createNestedObject(a);
    }

    debug();
}

String Preferences::getString(String key, String def)
{
    String value = object[key] | def;

    Serial.printf("Pref get %s = %s\n", key.c_str(), value.c_str() );

    return value;
}

void Preferences::putString(String key, String value)
{
    Serial.printf("Prefs %s=%s\n", key.c_str(), value.c_str());
    object[key] = value;

    debug();
}

void Preferences::end()
{
    save();
}

void Preferences::save()
{
    Serial.printf("Preferences save\n");

    File file = LittleFS.open("/Config.txt", "w+");
    debug();

    serializeJson(doc, file);
    
    file.close();

    Serial.printf("Preferences save end\n");
}

void Preferences::debug() {
    Serial.printf("[Preferences Contents]:\n");
     serializeJsonPretty(doc, Serial);
     Serial.printf("[/Preferences Contents]\n");
}

#endif