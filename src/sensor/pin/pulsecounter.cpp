//#include <Arduino.h>
#include "pulsecounter.h"
extern "C"
{
//#include "user_interface.h"
}

#include "debugging.h"
#include "time_utils.h"

/*
Electric pulse counters - optical, you know the OFF->ON->OFF 'flash' is a very fixed duration,
so a simple rising-edge interrupt with a debounce will work just fine.

For a gas meter, the reed switch is noisy and there's potentially an _extremely_ long time between
HI and LO. Looking at the pin value in the interrupt doesn't help, as it may be wobbling. So instead
count the transitions (CHANGE) and divide by 2.

*/

PulseCounter::PulseCounter(uint8_t pin, uint32_t debounce_time, int mode, bool pullup ) : BaseSensor(pin, debounce_time), m_mode(mode)
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

    if( pullup ) {
        pinMode(pin, INPUT_PULLUP);    
    }

    // Enable();

    m_pin_value = digitalRead(m_pin);

}

void PulseCounter::Process()
{
    int pinValue = digitalRead(m_pin);

    //if( pinValue != m_pin_value ) {
    //    Serial.print(pinValue?"1":"0");
    //}

    if( pinValue ) {
        m_deBouncer.reset();
        m_pin_value = 1;
    } else {
        
        if( m_pin_value == 1 ) {
           // Serial.print("-");
            if( m_deBouncer.debounceFilterPasses() ) {
             //   Serial.print("D");
                if( m_function != nullptr ) {
                        m_function(this);
                    }
                
                m_pin_value = 0;
            } else {
                
            }
            
        }
    }
    
}

IRAM_ATTR void PulseCounter::Interrupt() 
{

   if( !m_deBouncer.debounceFilterPasses() ) 
      return;
    
    m_count++;
    
    xQueueSendFromISR( m_isrQueue, &m_count, NULL);

}

void PulseCounter::Enable() {
    // OLD, pulse: attachInterruptArg(digitalPinToInterrupt(pin), interrupt, this, FALLING);
    // NEW: gas

    // You have to look for change otherwise it's impossible to tell whether the bouncing
    // signal is rising edge or falling edge.
    attachInterruptArg(digitalPinToInterrupt(m_pin), interruptFn, this, m_mode);
}

void PulseCounter::Disable() {
    detachInterrupt(digitalPinToInterrupt(m_pin));
}

