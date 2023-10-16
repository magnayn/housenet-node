//#include <Arduino.h>
#include "housenet_node.h"
#include "debugging.h"
#include <ArduinoJson.h>
#include "AsyncJson.h"
#include "time_utils.h"

//==============================================================================================================
// OneWire
//==============================================================================================================
HousenetOneWireElement::HousenetOneWireElement(HousenetNode *parent, String id, uint8_t pin)
: HousenetElement(parent, id), bus(pin)
{
ready = false;
}

void HousenetOneWireElement::Process() {
    char value[64];
    char topic[64];

    if( ready ) {

            static uint64_t lastUpdate = 0;
            uint64_t when = timeUtils.getTime();
            
            if (when > lastUpdate + 60000 ) 
            {
                for(auto const& state:  bus.getState()) {
                    sprintf(topic, "%2x%2x%2x%2x%2x%2x%2x%2x", 
                        state.address[0],state.address[1],state.address[2],state.address[3],
                        state.address[4],state.address[5],state.address[6],state.address[7]);
                    sprintf(value, "%.2f", state.temp);
                    publish(topic, value);
                }
                
                lastUpdate = when;
            }

        
    } else {
        bool more = bus.search();
        if( !more )
            ready = true;
    }
}
