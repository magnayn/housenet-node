//#include <Arduino.h>
#include "housenet_node.h"
#include "debugging.h"
#include <ArduinoJson.h>
#include "AsyncJson.h"
#include "time_utils.h"
#include "housenet_meter.h"

//==============================================================================================================
// Pulse Meter
//==============================================================================================================

const String HousenetMeterElement::TYPE      = "meter";


IRAM_ATTR HousenetMeterElement::HousenetMeterElement(HousenetNode *parent, String id, uint8_t pin, uint32_t debounceTime, int mode, bool pullup) : HousenetElement(parent, id) {   
    
    // Subscribe to set initial values..
    String stopic = "/housenet/" + node->station_id + "/" + GetType() + "/" + id + "/reading";

    Serial.println("Subscribe to " + stopic);

    node->client.subscribe(stopic);
    
    // TODO: create pulse_meter
    m_pulse_counter = new PulseCounter(pin, debounceTime, mode, pullup);

    /* pulse_meter->onChange(  [&](const PulseMeter *item)->void{
        meter_updated = true;
     });
    */

   auto g = std::bind(&HousenetMeterElement::OnPulseCounterEvent, this, std::placeholders::_1);
   m_pulse_counter->OnChangeCall(g);

   // pulse( pulse_meter );
    
}

void HousenetMeterElement::OnPulseCounterEvent(const BaseSensor* pm)
{
    Serial.println("Pulse");
    
    m_pulse_counter++;
    m_meter.increment();

    char value[64];

    sprintf(value, "%d", m_pulse_counter->GetCount());

    publish("pulse", value);

    if( m_meter.IsInitialized() ) {
        // Only send out if it's been init-ed. Publish as a "retained" value.
        sprintf(value, "%d", m_meter.GetValue());
        publish("reading", value, true, 0);
    }
}

String HousenetMeterElement::GetState( String channel ) {
    DynamicJsonDocument doc(256);

    doc["pulses"]             = m_pulse_counter->GetCount();
    doc["meter_initialized"]  = m_meter.IsInitialized();
    doc["meter"]              = m_meter.GetValue();
    
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
            m_meter.setValue(v);
        else {
            // treat as an increment, or a decrement to the value.
            m_meter.initialize(v);
        }
}

void HousenetMeterElement::Process() {
    m_pulse_counter->Process();
}

void HousenetMeterElement::OnMessage(String& topic, String& value)
{
    Serial.println("RCV " + topic + " = " + value);

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
            m_meter.setValue(v);
        else
            m_meter.initialize(v);                
}
