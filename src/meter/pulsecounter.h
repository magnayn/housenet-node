#ifndef __PULSECOUNTER_H__
#define __PULSECOUNTER_H__
#include "time_utils.h"
#include "debouncer.h"

#include <functional>

class PulseCounter;

typedef std::function<void(const PulseCounter *item)> PulseHandlerFunction;

class PulseCounter
{
public:
    PulseCounter(String id, uint8_t pin, uint32_t debounce_time);
    void onChange(PulseHandlerFunction onRequest);

    void signal_falling_edge();
    void signal_rising_edge();

    void enable();
    void disable();

    void interrupt();
private:
   

public:
    String id;
    uint8_t pin;

    DeBouncer deBouncer;
    
     volatile uint32_t events = 0;

    // Current Value (number of pulses)
    volatile uint32_t rise = 0;
    volatile uint32_t fall = 0;

    // pin state last time around
    volatile int state;

    PulseHandlerFunction _function;

    volatile int bummer;
    volatile int lo = 0;
    volatile int hi = 0;
};

#endif