//#include <Arduino.h>
#include "housenet_node.h"
#include "debugging.h"
#include <ArduinoJson.h>
#include "AsyncJson.h"
#include "time_utils.h"

//==============================================================================================================
// GPIO trigger : when asked, assert a GPIO line for a short length of time
//==============================================================================================================
const String HousenetGpioTriggerElement::TYPE = "gpiotrigger";

ICACHE_RAM_ATTR HousenetGpioTriggerElement::HousenetGpioTriggerElement(HousenetNode *parent, String id, uint8_t pin, unsigned long delayMs) : HousenetElement(parent, id) {   
    
    m_pin = pin;    
    m_delay = delayMs;
    digitalWrite(m_pin, 1);
    pinMode(m_pin, OUTPUT_OPEN_DRAIN);    
    
}

void HousenetGpioTriggerElement::OnMessage(String& topic, String& value)
{
    trigger();
}

void HousenetGpioTriggerElement::trigger()
{
    Serial.println("trigger");
    digitalWrite(m_pin, 0);
    delay(m_delay);
    digitalWrite(m_pin, 1);
    
}


// String HousenetMeterElement::GetState( String channel ) {
//     DynamicJsonDocument doc(256);

//     doc["pulses"] = pulse_meter->counter.value;
//     doc["meter_initialized"]  = pulse_meter->meter.initialized;
//     doc["meter"]  = pulse_meter->meter.value;
    
//     String data;
//     serializeJson(doc, data);
//     return data;
// }

void HousenetGpioTriggerElement::SetState( String channel, String value ) {
  trigger();
}
