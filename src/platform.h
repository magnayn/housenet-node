#ifndef __PLATFORM_H__
#define __PLATFORM_H__


#define _ESPASYNC_WIFIMGR_LOGLEVEL_    4

#include <Preferences.h>
#include <LittleFS.h>


#ifdef ESP32

//#include <LITTLEFS.h>

// #elif defined(ESP8266)
// #include "LittleFS.h"
// #define LITTLEFS LittleFS
#endif


#ifndef VERSION_NUMBER
#define VERSION_NUMBER 1.3.0
#endif


#endif