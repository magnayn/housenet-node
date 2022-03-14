#ifdef ESP8266
#include "preferences.h"

void Preferences::begin(String a, bool b)
{}

String Preferences::getString(String key, String def)
{
    return def;
}

void Preferences::end()
{}

#endif