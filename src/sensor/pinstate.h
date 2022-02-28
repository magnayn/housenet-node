#ifndef __PINSTATE_H__
#define __PINSTATE_H__
#include "time_utils.h"
#include <functional>

class PinState;

typedef std::function<void(const PinState *item)> PinStateHandlerFunction;

class PinState
{
public:
    PinState(uint8_t pin);
    void onChange(PinStateHandlerFunction onRequest);

    void enable();
    void disable();

    void interruptFunction();
public:
    
    uint8_t pin;

    int state;

    // For MQTT
    //uint32 last_broadcast = 0;

    PinStateHandlerFunction _function;

    TimeUtils timeUtils;
};

#endif