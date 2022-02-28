
#include <Arduino.h>
#include "onewirebus.h"
extern "C"
{
//#include "user_interface.h"
}

#include "debugging.h"
#include "time_utils.h"
#include <functional>

OneWireBus::OneWireBus(uint8_t pin) : ds(pin), sensors(&ds)
{

}

bool OneWireBus::search() {    
    DeviceAddress addr;

    if( !ds.search(addr) ) {

        ds.reset_search();
        return false;
    }
    DeviceAddress* data = new DeviceAddress[1];
    memcpy(data, addr, sizeof(addr));

    addresses.push_back( data );
    return true;
}

std::vector<BusItemState> OneWireBus::getState()
{
     std::vector<BusItemState> response;

    sensors.requestTemperatures();
    for(auto const& address: addresses) {
        float degC = sensors.getTempC(*address);
        BusItemState bis(*address, degC);
        response.push_back(bis);    
    }

     return response;
}