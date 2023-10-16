//#include <Arduino.h>
#include "BaseSensor.h"
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

IRAM_ATTR void interruptFn(void *arg)
{
    BaseSensor *item = (BaseSensor *)arg;
    item->Interrupt();
}

BaseSensor::BaseSensor(uint8_t pin, uint32_t debounce_time) : m_pin(pin), m_deBouncer(debounce_time)
{
   m_isrQueue = xQueueCreate( 10, sizeof(int) );
}

BaseSensor::~BaseSensor() {
    vQueueDelete( m_isrQueue );    
}

void BaseSensor::OnChangeCall(SensorChangeHandlerFunction onRequest) {
    m_function = onRequest;
}
    

void BaseSensor::Process() {    
    int i;

    while( xQueueReceive( m_isrQueue, &i, 0) == pdTRUE )
    {
        if( m_function != nullptr ) {
            m_function(this);
        }
    }
}