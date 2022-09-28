#ifndef __ONEWIREBUS_H__
#define __ONEWIREBUS_H__
#include "time_utils.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <functional>
#include <vector>
class OneWireBus;

typedef std::function<void(const OneWireBus *item)> OneWireBusHandlerFunction;

class BusItemState {
    public:
    BusItemState() {}
    public: 
        BusItemState(const DeviceAddress &a, float t) : temp(t)
        {
            memcpy(address, a, sizeof(address));
        }


    DeviceAddress address;
    float temp;
};

class OneWireBus
{
public:
    OneWireBus(uint8_t pin);

    bool search();
    std::vector<BusItemState> getState();
    
public:    
    OneWire ds;
    DallasTemperature sensors;

    std::vector<DeviceAddress*> addresses;

    OneWireBusHandlerFunction _function;

};

#endif