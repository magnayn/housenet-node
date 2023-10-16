
#include "meter.h"


/*
 * A meter is really just a counter with an initialize and a set, so that if the 'stored'
 value for the meter takes a while to arrive then it can 
 */


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