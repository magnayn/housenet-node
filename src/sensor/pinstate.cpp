#include <Arduino.h>
#include "pinstate.h"
extern "C"
{
//#include "user_interface.h"
}

#include "debugging.h"
#include "time_utils.h"

ICACHE_RAM_ATTR void interrupt_ps(void *arg)
{
    PinState *item = (PinState *)arg;
    item->interruptFunction();
}

PinState::PinState(uint8_t pin ) : pin(pin)
{    
    pinMode(pin, INPUT_PULLUP);    

    enable();
}

void PinState::enable() {

    attachInterruptArg(digitalPinToInterrupt(pin), interrupt_ps, this, CHANGE);
    state = digitalRead(pin);
}

void PinState::disable() {
    detachInterrupt(digitalPinToInterrupt(pin));
}

void PinState::onChange(PinStateHandlerFunction onRequest)
{
    _function = onRequest;
}

ICACHE_RAM_ATTR void PinState::interruptFunction()
{   
    int pin_value = digitalRead(pin);
    // ignore
    if( state == pin_value )
        return;

    state = pin_value;

    if (_function ) // Pull LO
    {   
        _function(this);
    }
}
