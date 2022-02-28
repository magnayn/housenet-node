#ifndef __PULSECOUNTER_H__
#define __PULSECOUNTER_H__
#include "time_utils.h"
#include <functional>

class PulseCounter;

typedef std::function<void(const PulseCounter *item)> PulseHandlerFunction;

class PulseCounter
{
public:
    PulseCounter(String id, uint8_t pin, uint32_t debounce_time);
    void onChange(PulseHandlerFunction onRequest);

    void increment_counter_falling();
    void increment_counter_change();

    void enable();
    void disable();

public:
    String id;
    uint8_t pin;

    // ms min between pulses
    uint64_t time_gap;

    // Current Value (number of pulses)
    uint32_t value = 0;

    // For Debounce
    uint64_t last_time = 0;


    int state;

    // For MQTT
    //uint32 last_broadcast = 0;

    PulseHandlerFunction _function;

    TimeUtils timeUtils;
};

#endif