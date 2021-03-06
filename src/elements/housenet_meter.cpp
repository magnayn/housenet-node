#include <Arduino.h>
#include "housenet_node.h"
#include "debugging.h"
#include <ArduinoJson.h>
#include "AsyncJson.h"
#include "time_utils.h"

//==============================================================================================================
// Pulse Meter
//==============================================================================================================

ICACHE_RAM_ATTR HousenetMeterElement::HousenetMeterElement(HousenetNode *parent, String id, uint8_t pin, uint32_t debounceTime) : HousenetElement(parent, id) {   
    
    // Subscribe to set initial values..
    String stopic = "/housenet/" + node->station_id + "/" + getType() + "/" + id + "/reading";

    Serial.println("Subscribe to " + stopic);

    node->client.subscribe(stopic);
    
    // TODO: create pulse_meter
    pulse_meter = new PulseMeter(id, pin, debounceTime);

     pulse_meter->onChange(  [&](const PulseMeter *item)->void{
        meter_updated = true;
     });

    pulse( pulse_meter );
    
}

String HousenetMeterElement::GetState( String channel ) {
    DynamicJsonDocument doc(256);

    doc["pulses"] = pulse_meter->counter.value;
    doc["meter_initialized"]  = pulse_meter->meter.initialized;
    doc["meter"]  = pulse_meter->meter.value;
    
    String data;
    serializeJson(doc, data);
    return data;
}

void HousenetMeterElement::SetState( String channel, String value ) {
  
        const char* txtValue = value.c_str();
        bool force = false;        
        int v = 0;

        if( txtValue[0] == '=' ) {
            force = true;
            txtValue = txtValue + 1;
        } 

        v = atoi(txtValue);

        
        // If we _force_ a value ( =1000 ) it will set the value regardless.
        // If we intialize, it will ignore if it already has bee initalised.

        if( force )
            pulse_meter->meter.setValue(v);
        else {
            // treat as an increment, or a decrement to the value.
            pulse_meter->meter.setValue( pulse_meter->meter.value + v);
        }
}

void HousenetMeterElement::process() {
    if( meter_updated ) {
        pulse( pulse_meter );
        meter_updated = false;
    }
}

void HousenetMeterElement::OnMessage(String& topic, String& value)
{
        const char* txtValue = value.c_str();
        bool force = false;        
        int v = 0;

        if( txtValue[0] == '=' ) {
            force = true;
            txtValue = txtValue + 1;
        }

        v = atoi(txtValue);

        
        // If we _force_ a value ( =1000 ) it will set the value regardless.
        // If we intialize, it will ignore if it already has bee initalised.

        if( force )
            pulse_meter->meter.setValue(v);
        else
            pulse_meter->meter.initialize(v);                
}
      

void HousenetMeterElement::pulse(const PulseMeter *pulseMeter)
{
    
    char value[64];

    sprintf(value, "%d", pulseMeter->counter.value);

    publish("pulse", value);

    if( pulseMeter->meter.initialized ) {
        // Only send out if it's been init-ed. Publish as a "retained" value.

        
        sprintf(value, "%d", pulseMeter->meter.value);
        publish("reading", value, true, 0);
    }
}