#include <Arduino.h>
#include "housenet_node.h"
#include "debugging.h"
#include <ArduinoJson.h>
#include "AsyncJson.h"
#include "time_utils.h"

//==============================================================================================================
// State
//==============================================================================================================

HousenetStateElement::HousenetStateElement(HousenetNode *parent, String id, uint8_t pin)
: HousenetElement(parent, id), pinState(pin)
{

    
    pinState.onChange([&](const PinState* state)->void{        
        callback(state);
    });

}

ICACHE_RAM_ATTR void HousenetStateElement::callback(const PinState* state)
{
 ready = true;
}
    
void HousenetStateElement::process() {
    if( ready ) {
         ready = false;
         Serial.println("Process");
         Serial.println(pinState.state?"0":"1");

         publish("value", pinState.state?"0":"1");
         Serial.println("Process Done");
    }
}

String HousenetStateElement::GetState( String channel ) {
    DynamicJsonDocument doc(256);

    doc["state"] = pinState.state?false:true;
    
    String data;
    serializeJson(doc, data);
    return data;
}