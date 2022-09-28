#include <Arduino.h>
#include "housenet_node.h"
#include "debugging.h"
#include <ArduinoJson.h>
#include "AsyncJson.h"
#include "time_utils.h"

//==============================================================================================================
// GPIO trigger : when asked, assert a GPIO line for a short length of time
//==============================================================================================================
const String HousenetGpioTriggerElement::TYPE = "gpiotrigger";

ICACHE_RAM_ATTR HousenetGpioTriggerElement::HousenetGpioTriggerElement(HousenetNode *parent, String id, uint8_t pin) : HousenetElement(parent, id) {   
    
    m_pin = pin;    
    digitalWrite(m_pin, 1);
    pinMode(m_pin, OUTPUT_OPEN_DRAIN);    
    
}

void HousenetGpioTriggerElement::OnMessage(String& topic, String& value)
{
    Serial.println("trigger");
    digitalWrite(m_pin, 0);
    delay(300);
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

// void HousenetMeterElement::SetState( String channel, String value ) {
  
//         const char* txtValue = value.c_str();
//         bool force = false;        
//         int v = 0;

//         if( txtValue[0] == '=' ) {
//             force = true;
//             txtValue = txtValue + 1;
//         } 

//         v = atoi(txtValue);

        
//         // If we _force_ a value ( =1000 ) it will set the value regardless.
//         // If we intialize, it will ignore if it already has bee initalised.

//         if( force )
//             pulse_meter->meter.setValue(v);
//         else {
//             // treat as an increment, or a decrement to the value.
//             pulse_meter->meter.setValue( pulse_meter->meter.value + v);
//         }
// }

// void HousenetMeterElement::process() {
//     if( meter_updated ) {
//         pulse( pulse_meter );
//         meter_updated = false;
//     }
// }


      

// void HousenetMeterElement::pulse(const PulseMeter *pulseMeter)
// {
    
//     char value[64];

//     sprintf(value, "%d", pulseMeter->counter.value);

//     publish("pulse", value);

//     if( pulseMeter->meter.initialized ) {
//         // Only send out if it's been init-ed. Publish as a "retained" value.

        
//         sprintf(value, "%d", pulseMeter->meter.value);
//         publish("reading", value, true, 0);
//     }
// }