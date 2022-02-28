#ifndef __PULSEMETER_H__
#define __PULSEMETER_H__

#include "pulsecounter.h"
#include "meter.h"
#include <functional>
class PulseMeter;

typedef std::function<void(const PulseMeter *item)> PulseMeterFunction;
    
class PulseMeter {

    public:
        PulseMeter(String id, uint8_t pin, uint32_t debouce_time);

        void onChange(PulseMeterFunction onChange) {
            _function = onChange;
        }

    private:
        void onPulse(const PulseCounter* pc);

    public:
        PulseCounter counter;
        Meter        meter;

        PulseMeterFunction _function;
};

#endif