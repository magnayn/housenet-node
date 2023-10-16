#ifndef __PULSECOUNTER_H__
#define __PULSECOUNTER_H__
#include "../basesensor.h"
#include "time_utils.h"
#include "../../utils/debouncer.h"

#include <functional>

class PulseCounter;

class PulseCounter : public BaseSensor
{
public:
    PulseCounter(uint8_t pin, uint32_t debounce_time, int mode, bool pullup);

    void Enable();
    void Disable();

    void Interrupt();

    virtual void Process();

    int GetCount() { 
        return m_count;
    }

private:
    int   m_count = 0;

    int   m_mode;

    int   m_pin_value;
};

#endif