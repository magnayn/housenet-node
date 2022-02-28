#ifndef __EMONTX_H__
#define __EMONTX_H__

#include <ArduinoJson.h>
#include <functional>

class EmonMessage
{
public:
    bool parse(const String &str);

    DynamicJsonDocument ToJson() const;

    int msg_seq = 0;
    float v = 0;
    int power[4] = {0, 0, 0, 0};
    int energy_in[4] = {0, 0, 0, 0};
    int energy_out[4] = {0, 0, 0, 0};
};

typedef std::function<void(const EmonMessage &msg)> EmonTXHandlerFunction;

class EmonTX
{
protected:
public:
    EmonTX();

    void process();
    void onMessage(EmonTXHandlerFunction onRequest);

protected:
    void processMessage(const String &message);

private:
    void processBuffer();

private:
    String _message;
    EmonTXHandlerFunction _function = nullptr;

    HardwareSerial& serialPort;
};

#endif