#ifndef __METER_H__
#define __METER_H__

#include <Arduino.h>

class Meter
{
public:
    
    void initialize(uint32_t value);
    void setValue(uint32_t value);

    void increment();

    uint32_t GetValue() { 
        return value;
    }

    bool IsInitialized() {
        return initialized;
    }
private:        
    bool initialized = false;

    // Current Value 
    uint32_t value = 0;
};



#endif