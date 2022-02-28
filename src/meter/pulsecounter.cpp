#include <Arduino.h>
#include "pulsecounter.h"
extern "C"
{
//#include "user_interface.h"
}

#include "debugging.h"
#include "time_utils.h"

ICACHE_RAM_ATTR void interrupt(void *arg)
{
    PulseCounter *item = (PulseCounter *)arg;
    item->increment_counter_change();
}

PulseCounter::PulseCounter(String id, uint8_t pin, uint32_t debounce_time ) : id(id), pin(pin), time_gap(debounce_time)
{
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

    pinMode(pin, INPUT_PULLUP);    

    enable();
}

void PulseCounter::enable() {
    // OLD, pulse: attachInterruptArg(digitalPinToInterrupt(pin), interrupt, this, FALLING);
    // NEW: gas

    // You have to look for change otherwise it's impossible to tell whether the bouncing
    // signal is rising edge or falling edge.
    attachInterruptArg(digitalPinToInterrupt(pin), interrupt, this, CHANGE);
    state = digitalRead(pin);
}

void PulseCounter::disable() {
    detachInterrupt(digitalPinToInterrupt(pin));
}

void PulseCounter::onChange(PulseHandlerFunction onRequest)
{
    _function = onRequest;
}

ICACHE_RAM_ATTR void PulseCounter::increment_counter_change()
{
    // char str[40];

    int pin_value = digitalRead(pin);
    // ignore
    if( state == pin_value )
        return;

    state = pin_value;

    uint64_t time = timeUtils.getTime();

    if (time > (last_time + time_gap))
    {
        last_time = time;

        if( state == 0 ) {
            value++;
        }

        if (_function != NULL && state == 0 ) // Pull LO
        {
            _function(this);
        }
    }
}

void PulseCounter::increment_counter_falling()
{
    // char str[40];

    // int pin_value = digitalRead(pin);

    uint64_t time = timeUtils.getTime();

    if (time > (last_time + time_gap))
    {
        value++;
        last_time = time;

        // sprintf(str, "%lld [%s:%d] : %d", time, id.c_str(), pin_value, value);
        // Debugging.println(str);

        if (_function != NULL)
        {
            _function(this);
        }
    }
    else
    {
        // sprintf(str, "%lld [%s:%d] : --", time, id.c_str(), pin_value);
        // Debugging.println(str);
    }
}
