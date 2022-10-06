#include <Arduino.h>
#include "housenet_node.h"
#include "debugging.h"
#include <ArduinoJson.h>
#include "AsyncJson.h"
#include "time_utils.h"
#include "housenet_opentherm.h"


void OTReading::WriteToJson(JsonObject& doc) const
{
  doc["ch"] =  ch;
  doc["dhw"] =  dhw;
  doc["flame"] =  flame;

  // Comes from us
  doc["setPoint"] = setPoint;

  doc["temperature"] =  temperature;

  // Obs:
  doc["relModLevel"] =   relModLevel;
  
  doc["pressure"] =  pressure;
  doc["dhwTemp"] =  dhwTemp;
  doc["tempReturn"] =  tempReturn;
  
  doc["burnerStarts"] =  burnerStarts;
  doc["dhwBurnerStarts"] =  dhwBurnerStarts;
  doc["burnerOperationHours"] =  burnerOperationHours;
  
  doc["setPointMax"] =  setPointMax;
}

//==============================================================================================================
// OpenTherm
//==============================================================================================================

const String HousenetOpenthermElement::TYPE = "opentherm";

ICACHE_RAM_ATTR HousenetOpenthermElement::HousenetOpenthermElement(HousenetNode *parent, String id, uint8_t pinIn, uint8_t pinOut)
    : HousenetElement(parent, id), ot((int)pinIn, (int)pinOut)
{

  ot.begin(
      [&](void) -> void
      {
        ot.handleInterrupt();
      });
}

String HousenetOpenthermElement::GetState(String channel)
{
  DynamicJsonDocument doc(256);

  doc["setpoint"] = setpoint;
    
  auto status = doc.createNestedObject("reading");

  OTReading reading = GetReading();
  reading.WriteToJson(status);
  
  String data;
  serializeJson(doc, data);
  return data;
}

void HousenetOpenthermElement::SetState(String channel, String value)
{
  setpoint = atof(value.c_str());
}

OTReading HousenetOpenthermElement::GetReading()
{
  OTReading r;
   bool enableCentralHeating = true;
    bool enableHotWater = true;
    bool enableCooling = false;

  unsigned long response = ot.setBoilerStatus(enableCentralHeating, enableHotWater, enableCooling);
    OpenThermResponseStatus responseStatus = ot.getLastResponseStatus();
    if (responseStatus == OpenThermResponseStatus::SUCCESS)
    {
    }

      r.ch = ot.isCentralHeatingActive(response);
      r.dhw = ot.isHotWaterActive(response);
      r.flame = ot.isFlameOn(response);

  r.temperature = ot.getBoilerTemperature();

  r.setPoint = setpoint;

  r.relModLevel = getF88(OpenThermMessageID::RelModLevel);

  r.pressure = getF88(OpenThermMessageID::CHPressure);

  r.dhwTemp = getF88(OpenThermMessageID::Tdhw);
  r.tempReturn = getF88(OpenThermMessageID::Tret); // Negative?!

  r.burnerStarts = getU16(OpenThermMessageID::BurnerStarts);
  r.dhwBurnerStarts = getU16(OpenThermMessageID::DHWBurnerStarts);
  r.burnerOperationHours = getU16(OpenThermMessageID::BurnerOperationHours);

  r.setPointMax = getF88(OpenThermMessageID::MaxTSet);

  return r;
}

void HousenetOpenthermElement::publishReading(OTReading &thisReading)
{

  publish("ch", thisReading.ch ? "1" : "0");
  publish("dhw", thisReading.dhw ? "1" : "0");
  publish("flame", thisReading.flame ? "1" : "0");

  publishf("temperature", thisReading.temperature);

  publishf("ch/setpoint/controller", thisReading.setPoint);

  publishf("modulation_rate", thisReading.relModLevel);

  publishf("ch/pressure", thisReading.pressure);
  publishf("dhw/temperature", thisReading.dhwTemp);
  publishf("ch/return_temperature", thisReading.tempReturn); // Negative?!

  publishu16("burner/starts", thisReading.burnerStarts);
  publishu16("dhw/burner/starts", thisReading.dhwBurnerStarts);
  publishu16("burner/hours", thisReading.burnerOperationHours);
  publishf("ch/setpoint/max", thisReading.setPointMax);
}

void HousenetOpenthermElement::process()
{

  new_ts = millis();

  if (new_ts - ts > 1000)
  {
    ot.setBoilerTemperature(setpoint);
    
    // Set/Get Boiler Status
    bool enableCentralHeating = true;
    bool enableHotWater = true;
    bool enableCooling = false;

    OTReading thisReading;

    unsigned long response = ot.setBoilerStatus(enableCentralHeating, enableHotWater, enableCooling);
    OpenThermResponseStatus responseStatus = ot.getLastResponseStatus();
    if (responseStatus == OpenThermResponseStatus::SUCCESS)
    {

      thisReading.ch = ot.isCentralHeatingActive(response);
      thisReading.dhw = ot.isHotWaterActive(response);
      thisReading.flame = ot.isFlameOn(response);

      publish("ch", thisReading.ch ? "1" : "0");
      publish("dhw", thisReading.dhw ? "1" : "0");
      publish("flame", thisReading.flame ? "1" : "0");
    }
    if (responseStatus == OpenThermResponseStatus::NONE)
    {
      Serial.println("Error: OpenTherm is not initialized");
      return;
    }
    else if (responseStatus == OpenThermResponseStatus::INVALID)
    {
      Serial.println("Error: Invalid response " + String(response, HEX));
      return;
    }
    else if (responseStatus == OpenThermResponseStatus::TIMEOUT)
    {
      Serial.println("Error: Response timeout");
      return;
    }

    // Various!
    char data[50];

    thisReading.temperature = ot.getBoilerTemperature();
    sprintf(data, "%.2f", thisReading.temperature);
    publish("temperature", data);
    

    thisReading.setPoint = setpoint;

    //  publish_f88(OpenThermMessageID::TSet,   "ch/setpoint");

    sprintf(data, "%.2f", setpoint);

    publish("ch/setpoint/controller", data);

    thisReading.relModLevel = publish_f88(OpenThermMessageID::RelModLevel, "modulation_rate");

    thisReading.pressure = publish_f88(OpenThermMessageID::CHPressure, "ch/pressure");
    // publish_f88(OpenThermMessageID::DHWFlowRate,  "dhw/flow_rate");
    publish_f88(OpenThermMessageID::Tdhw, "dhw/temperature");
    // publish_f88(OpenThermMessageID::Toutside,     "outside_temperature");
    publish_f88(OpenThermMessageID::Tret, "ch/return_temperature"); // Negative?!
    publish_raw(OpenThermMessageID::Tret, "ch/return_temperature/raw");

    // publish_s16(OpenThermMessageID::Texhaust,         "exhaust/temperature");

    publish_u16(OpenThermMessageID::BurnerStarts, "burner/starts");
    // publish_u16(OpenThermMessageID::CHPumpStarts,             "ch/pump/starts");
    //  publish_u16(OpenThermMessageID::DHWPumpValveStarts,       "dhw/pump_valve/starts");
    publish_u16(OpenThermMessageID::DHWBurnerStarts, "dhw/burner/starts");
    publish_u16(OpenThermMessageID::BurnerOperationHours, "burner/hours");
    // publish_u16(OpenThermMessageID::CHPumpOperationHours,         "ch/pump/hours");
    // publish_u16(OpenThermMessageID::DHWPumpValveOperationHours,   "dhw/pump_valve/hours");
    // publish_u16(OpenThermMessageID::DHWBurnerOperationHours,      "dhw/burner/hours");

    // publish_f88(OpenThermMessageID::TdhwSet,   "dhw/setpoint");
    publish_f88(OpenThermMessageID::MaxTSet, "ch/setpoint/max");
    // publish_f88(OpenThermMessageID::Hcratio,   "otc/heat_curve");

    //  publish_f88(OpenThermMessageID::OpenThermVersionMaster,   "opentherm/master/version");
    //  publish_f88(OpenThermMessageID::OpenThermVersionSlave,   "opentherm/slave/version");

    // Temp

    ts = new_ts;
  }
}

float HousenetOpenthermElement::getF88(OpenThermMessageID id)
{
  unsigned long value = ot.sendRequest(ot.buildRequest(OpenThermMessageType::READ_DATA, id, 0));

  OpenThermResponseStatus responseStatus = ot.getLastResponseStatus();
  if (responseStatus == OpenThermResponseStatus::SUCCESS)
  {
    return ot.getFloat(value);
  }

  return 0;
}

int16_t HousenetOpenthermElement::getS16(OpenThermMessageID id)
{
  unsigned long value = ot.sendRequest(ot.buildRequest(OpenThermMessageType::READ_DATA, id, 0));

  OpenThermResponseStatus responseStatus = ot.getLastResponseStatus();
  if (responseStatus == OpenThermResponseStatus::SUCCESS)
  {
    return (int16_t)ot.getUInt(value);
  }

  return 0;
}

uint16_t HousenetOpenthermElement::getU16(OpenThermMessageID id)
{
  unsigned long value = ot.sendRequest(ot.buildRequest(OpenThermMessageType::READ_DATA, id, 0));

  OpenThermResponseStatus responseStatus = ot.getLastResponseStatus();
  if (responseStatus == OpenThermResponseStatus::SUCCESS)
  {
    return ot.getUInt(value);
  }

  return 0;
}

unsigned long HousenetOpenthermElement::getRAW(OpenThermMessageID id)
{
  unsigned long value = ot.sendRequest(ot.buildRequest(OpenThermMessageType::READ_DATA, id, 0));

  OpenThermResponseStatus responseStatus = ot.getLastResponseStatus();
  if (responseStatus == OpenThermResponseStatus::SUCCESS)
  {
    return value;
  }

  return 0;
}

unsigned long HousenetOpenthermElement::publish_f88(OpenThermMessageID id, const char *topic)
{

  char data[128];
  unsigned long value = ot.sendRequest(ot.buildRequest(OpenThermMessageType::READ_DATA, id, 0));

  OpenThermResponseStatus responseStatus = ot.getLastResponseStatus();
  if (responseStatus == OpenThermResponseStatus::SUCCESS)
  {

    // Isn't actually a temperature, doesn't really matter it's f8.8
    snprintf(data, 128, "%.2f", ot.getFloat(value));

    publish(topic, data);
    return value;
  }
  else
  {
    snprintf(data, 128, "Channel %s Error %s", topic, ot.statusToString(responseStatus));
    publish("error", data);
    return -1;
  }
}

void HousenetOpenthermElement::publishf(String name, float value)
{
  char data[30];
  snprintf(data, 30, "%.2f", value);
  publish(name, data);
}

void HousenetOpenthermElement::publishu16(String name, uint16_t value)
{
  char data[30];
  snprintf(data, 30, "%u", value);
  publish(name, data);
}

int16_t HousenetOpenthermElement::publish_s16(OpenThermMessageID id, const char *topic)
{

  char data[128];
  unsigned long value = ot.sendRequest(ot.buildRequest(OpenThermMessageType::READ_DATA, id, 0));

  OpenThermResponseStatus responseStatus = ot.getLastResponseStatus();
  if (responseStatus == OpenThermResponseStatus::SUCCESS)
  {

    int16_t v2 = (int16_t)ot.getUInt(value);

    // Isn't actually a temperature, doesn't really matter it's f8.8
    snprintf(data, 128, "%d", v2);

    publish(topic, data);
    return v2;
  }
  else
  {
    snprintf(data, 128, "Channel %s Error %s", topic, ot.statusToString(responseStatus));
    publish("error", data);
    return -1;
  }
}

unsigned long HousenetOpenthermElement::publish_u16(OpenThermMessageID id, const char *topic)
{
  char data[128];
  unsigned long value = ot.sendRequest(ot.buildRequest(OpenThermMessageType::READ_DATA, id, 0));

  OpenThermResponseStatus responseStatus = ot.getLastResponseStatus();
  if (responseStatus == OpenThermResponseStatus::SUCCESS)
  {

    snprintf(data, 128, "%u", ot.getUInt(value));

    publish(topic, data);
    return value;
  }
  else
  {
    snprintf(data, 128, "Channel %s Error %s", topic, ot.statusToString(responseStatus));
    publish("error", data);
    return -1;
  }
}

void HousenetOpenthermElement::publish_raw(OpenThermMessageID id, const char *topic)
{

  char data[128];
  unsigned long value = ot.sendRequest(ot.buildRequest(OpenThermMessageType::READ_DATA, id, 0));

  OpenThermResponseStatus responseStatus = ot.getLastResponseStatus();
  if (responseStatus == OpenThermResponseStatus::SUCCESS)
  {

    snprintf(data, 128, "%x %lx", ot.getUInt(value), value);

    publish(topic, data);
  }
  else
  {
    snprintf(data, 128, "Channel %s Error %s", topic, ot.statusToString(responseStatus));
    publish("error", data);
  }
}
