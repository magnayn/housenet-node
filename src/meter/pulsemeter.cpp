#include <Arduino.h>
#include "pulsemeter.h"
#include <functional>

IRAM_ATTR PulseMeter::PulseMeter(String id, uint8_t pin, uint32_t debounce_time) : counter(id, pin, debounce_time), meter(id) {
//    counter.onChange([&](const PulseCounter *pc) { onPulse(pc); });
    auto g = std::bind(&PulseMeter::onPulse, this, std::placeholders::_1);
    
    counter.onChange(g);
}

IRAM_ATTR void PulseMeter::onPulse(const PulseCounter* pc) {
    
    meter.increment();

    if (_function != NULL)
    {
        _function(this);
    }
}
