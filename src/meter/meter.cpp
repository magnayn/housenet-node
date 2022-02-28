#include <Arduino.h>
#include "meter.h"

Meter::Meter(String id) : id(id) {

}

void Meter::initialize(uint32_t setting) {

    // If we haven't been initialized, then append the counts up to this point
    // Otherwise just set to the absolute value.
    if( !initialized ) {
        value = value + setting;
        initialized = true;
    }    
}

void Meter::setValue(uint32_t setting) {
    value = setting;
    initialized = true;        
}

ICACHE_RAM_ATTR void Meter::increment() {
    value = value + 1;
}