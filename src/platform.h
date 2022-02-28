#ifndef __PLATFORM_H__
#define __PLATFORM_H__


#define _ESPASYNC_WIFIMGR_LOGLEVEL_    4

#ifdef ESP32
#include <Preferences.h>
#include <LITTLEFS.h>

#elif defined(ESP8266)
#include "esp8266/preferences.h"
#include "LittleFS.h"
#define LITTLEFS LittleFS
#endif


#endif