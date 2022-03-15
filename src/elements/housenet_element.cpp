#include <Arduino.h>
#include "housenet_node.h"


const String HousenetStateElement::TYPE     = "state";

const String HousenetMeterElement::TYPE      = "meter";
const String HousenetEmonTXElement::TYPE     = "emontx";
const String HousenetOneWireElement::TYPE    = "onewire";

//==============================================================================================================
// Base element
//==============================================================================================================
bool HousenetElement::publish(const String &topic, const String &payload)
{
  String comb = "/housenet/" + node->station_id + "/" + getType() + "/" + id + "/" + topic;
  return node->client.publish(comb, payload);
  
}

bool HousenetElement::publish(const char topic[], const char payload[], bool retained, int qos) {
   return node->client.publish("/housenet/" + node->station_id + "/" + getType() + "/" + id + "/" + topic, payload, retained, qos);

}

String HousenetElement::GetState( String channel )
{
    return "{}";
}

void HousenetElement::SetState( String channel, String data )
{

}
