#include <Arduino.h>
#include "housenet_node.h"
#include "debugging.h"
#include <ArduinoJson.h>
#include "AsyncJson.h"
#include "time_utils.h"


//==============================================================================================================
// OpenTherm
//==============================================================================================================

class OTReading
{
public:
  bool ch;
  bool dhw;
  bool flame;

  // Comes from us
  float setPoint;

  float temperature;

  // Obs:
  unsigned long relModLevel;
  
};

const String HousenetOpenthermElement::TYPE = "opentherm";

HousenetOpenthermElement::HousenetOpenthermElement(HousenetNode *parent, String id, uint8_t pinIn, uint8_t pinOut) 
    : HousenetElement(parent, id), ot((int)pinIn, (int)pinOut)
{

    ot.begin(
    [&](void) -> void {
        ot.handleInterrupt(); 
    });
                    
}

void HousenetOpenthermElement::process() {
     new_ts = millis();


  if (new_ts - ts > 1000) {  
  
  //Set/Get Boiler Status
  bool enableCentralHeating = true;
  bool enableHotWater = true;
  bool enableCooling = false;

  OTReading thisReading;
  
  unsigned long response = ot.setBoilerStatus(enableCentralHeating, enableHotWater, enableCooling);
  OpenThermResponseStatus responseStatus = ot.getLastResponseStatus();
  if (responseStatus == OpenThermResponseStatus::SUCCESS) {   

    thisReading.ch = ot.isCentralHeatingActive(response);
    thisReading.dhw = ot.isHotWaterActive(response);
    thisReading.flame = ot.isFlameOn(response);
    
    publish("ch",    thisReading.ch?"1":"0" );
    publish("dhw",   thisReading.dhw?"1":"0" );
    publish("flame", thisReading.flame?"1":"0" );
  }
  if (responseStatus == OpenThermResponseStatus::NONE) {
    Serial.println("Error: OpenTherm is not initialized");
  }
  else if (responseStatus == OpenThermResponseStatus::INVALID) {
    Serial.println("Error: Invalid response " + String(response, HEX));
  }
  else if (responseStatus == OpenThermResponseStatus::TIMEOUT) {
    Serial.println("Error: Response timeout");
  }

  // Various!
  unsigned long value;
  char data[50];

  thisReading.temperature = ot.getBoilerTemperature();
  sprintf(data, "%.2f", thisReading.temperature);  
  publish("temperature", data);
  ot.setBoilerTemperature(setpoint);

  thisReading.setPoint = setpoint;

 //  publish_f88(OpenThermMessageID::TSet,   "ch/setpoint");

  sprintf(data, "%.2f", setpoint);
  publish("ch/setpoint/controller", data);

  thisReading.relModLevel = publish_f88(OpenThermMessageID::RelModLevel, "modulation_rate");

  publish_f88(OpenThermMessageID::CHPressure,   "ch/pressure");
 // publish_f88(OpenThermMessageID::DHWFlowRate,  "dhw/flow_rate");
  publish_f88(OpenThermMessageID::Tdhw,         "dhw/temperature");
 // publish_f88(OpenThermMessageID::Toutside,     "outside_temperature");
  publish_f88(OpenThermMessageID::Tret,         "ch/return_temperature"); // Negative?!
  publish_raw(OpenThermMessageID::Tret,         "ch/return_temperature/raw"); 
  
 // publish_s16(OpenThermMessageID::Texhaust,         "exhaust/temperature");
  

  publish_u16(OpenThermMessageID::BurnerStarts,             "burner/starts");
  // publish_u16(OpenThermMessageID::CHPumpStarts,             "ch/pump/starts");
 //  publish_u16(OpenThermMessageID::DHWPumpValveStarts,       "dhw/pump_valve/starts");
  publish_u16(OpenThermMessageID::DHWBurnerStarts,          "dhw/burner/starts");
  publish_u16(OpenThermMessageID::BurnerOperationHours,     "burner/hours");
 // publish_u16(OpenThermMessageID::CHPumpOperationHours,         "ch/pump/hours");
 // publish_u16(OpenThermMessageID::DHWPumpValveOperationHours,   "dhw/pump_valve/hours");
 // publish_u16(OpenThermMessageID::DHWBurnerOperationHours,      "dhw/burner/hours");
 

 // publish_f88(OpenThermMessageID::TdhwSet,   "dhw/setpoint");
  publish_f88(OpenThermMessageID::MaxTSet,   "ch/setpoint/max");
 // publish_f88(OpenThermMessageID::Hcratio,   "otc/heat_curve");

 //  publish_f88(OpenThermMessageID::OpenThermVersionMaster,   "opentherm/master/version");
 //  publish_f88(OpenThermMessageID::OpenThermVersionSlave,   "opentherm/slave/version");
  
  // Temp
  
  ts = new_ts;
  }
}

unsigned long HousenetOpenthermElement::publish_f88(OpenThermMessageID id, char* topic) {
  char data[30];
  unsigned long value = ot.sendRequest(ot.buildRequest(OpenThermMessageType::READ_DATA, id,0));
  
  OpenThermResponseStatus responseStatus = ot.getLastResponseStatus();
  if (responseStatus == OpenThermResponseStatus::SUCCESS) {   

     // Isn't actually a temperature, doesn't really matter it's f8.8
     sprintf(data, "%.2f", ot.getFloat(value) );
      
     publish(topic, data );
     return value;
  } else {
      sprintf(data, "Channel %s Error %s", topic, ot.statusToString(responseStatus) );
      publish("error", data);
      return -1;
  }
  
}


int16_t HousenetOpenthermElement::publish_s16(OpenThermMessageID id, char* topic) {
  char data[30];
  unsigned long value = ot.sendRequest(ot.buildRequest(OpenThermMessageType::READ_DATA, id,0));
  
  OpenThermResponseStatus responseStatus = ot.getLastResponseStatus();
  if (responseStatus == OpenThermResponseStatus::SUCCESS) {   

    int16_t v2 = (int16_t)ot.getUInt(value);

     // Isn't actually a temperature, doesn't really matter it's f8.8
     sprintf(data, "%d", v2 );
      
     publish(topic, data );
     return v2;
  } else {
      sprintf(data, "Channel %s Error %s", topic, ot.statusToString(responseStatus) );
      publish("error", data);
      return -1;
  }
  
}

unsigned long HousenetOpenthermElement::publish_u16(OpenThermMessageID id, char* topic) {
  char data[30];
  unsigned long value = ot.sendRequest(ot.buildRequest(OpenThermMessageType::READ_DATA, id,0));
  
  OpenThermResponseStatus responseStatus = ot.getLastResponseStatus();
  if (responseStatus == OpenThermResponseStatus::SUCCESS) {   

     sprintf(data, "%u", ot.getUInt(value) );
      
     publish(topic, data );
     return value;
  } else {
      sprintf(data, "Channel %s Error %s", topic, ot.statusToString(responseStatus) );
      publish("error", data);
      return -1;
  }
  
}


void HousenetOpenthermElement::publish_raw(OpenThermMessageID id, char* topic) {
  char data[30];
  unsigned long value = ot.sendRequest(ot.buildRequest(OpenThermMessageType::READ_DATA, id,0));
  
  OpenThermResponseStatus responseStatus = ot.getLastResponseStatus();
  if (responseStatus == OpenThermResponseStatus::SUCCESS) {   

     sprintf(data, "%x %x", ot.getUInt(value), value );
      
     publish(topic, data );
  } else {
      sprintf(data, "Channel %s Error %s", topic, ot.statusToString(responseStatus) );
      publish("error", data);
  }
  
}

