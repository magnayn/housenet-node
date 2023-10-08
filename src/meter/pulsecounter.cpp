#include <Arduino.h>
#include "pulsecounter.h"
extern "C"
{
//#include "user_interface.h"
}

#include "debugging.h"
#include "time_utils.h"

IRAM_ATTR void interruptFn(void *arg)
{
    PulseCounter *item = (PulseCounter *)arg;
    item->interrupt();
}

PulseCounter::PulseCounter(String id, uint8_t pin, uint32_t debounce_time ) : id(id), pin(pin), deBouncer(debounce_time), bummer(0)
{

    // GPIO23 pullup doesn't work?
    
    /* Electric: 100A = 24kW.
 *
 * 1Wh = 1 flash
 * 1kWh = 1000 flashes
 * 24kWh = 24000 flashes, in 3600 seconds
 *
 * = 24000/3600 flashes per second (6.667)
 *
 * 1 flash every 0.15 secs.
 *
 * ~ set debounce to 0.1 secs = 0.1* 1000000 = 100000uS
 *
 *
 */
    pinMode(pin, INPUT); 
    pinMode(pin, INPUT_PULLUP);    

    enable();
}

IRAM_ATTR void PulseCounter::interrupt() 
{
    int pin_value = digitalRead(pin);

    events++;
    if( pin_value ) {
        hi ++;
    } else {
        lo ++;
    }

    if( deBouncer.debounceFilterPasses() ) {
        
        if( state == 1 && pin_value == 0 ) {
            signal_falling_edge();
        } else if( state == 0 && pin_value ) {
            signal_rising_edge();
        } else {
            // I worry that the interrupt is triggered but the signal goes back before we 'see' it
            bummer++;

            // If so, ignore it and keep looking as it should change again.
            deBouncer.reset();
        }

        state = pin_value;
    }
}

void PulseCounter::enable() {
    // OLD, pulse: attachInterruptArg(digitalPinToInterrupt(pin), interrupt, this, FALLING);
    // NEW: gas

    // You have to look for change otherwise it's impossible to tell whether the bouncing
    // signal is rising edge or falling edge.
    attachInterruptArg(digitalPinToInterrupt(pin), interruptFn, this, CHANGE);
    state = digitalRead(pin);
}

void PulseCounter::disable() {
    detachInterrupt(digitalPinToInterrupt(pin));
}

void PulseCounter::onChange(PulseHandlerFunction onRequest)
{
    _function = onRequest;
}

IRAM_ATTR void PulseCounter::signal_falling_edge()
{
   
    fall++;

    if (_function != NULL ) // Pull LO
    {
        _function(this);
    }

}

void PulseCounter::signal_rising_edge()
{
    rise++;
}
