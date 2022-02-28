#ifndef __METER_H__
#define __METER_H__


class Meter
{
public:
    Meter(String id);
    
    void initialize(uint32_t value);
    void setValue(uint32_t value);

    void increment();

public:    
    String id;

    bool initialized = false;

    // Current Value 
    uint32_t value = 0;
};



#endif