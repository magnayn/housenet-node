#ifndef __DEBUGGING_H__
#define __DEBUGGING_H__

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
class _Debugging
{
public:
    _Debugging();

    void print(const String &data);
    void println(const String &data);

public:
    AsyncWebSocketClient *websocket;
};

extern _Debugging Debugging;

#endif