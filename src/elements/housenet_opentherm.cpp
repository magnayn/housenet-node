#include "housenet_node.h"
#include "debugging.h"
#include <ArduinoJson.h>
#include "AsyncJson.h"
#include "time_utils.h"
#include "housenet_opentherm.h"

void OTReading::WriteToJson(JsonObject &doc) const
{
  doc["ch"] = ch;
  doc["dhw"] = dhw;
  doc["flame"] = flame;

  // Comes from us
  doc["setPoint"] = setPoint;

  doc["temperature"] = temperature;

  // Obs:
  doc["relModLevel"] = relModLevel;

  doc["pressure"] = pressure;
  doc["dhwTemp"] = dhwTemp;
  doc["tempReturn"] = tempReturn;

  doc["burnerStarts"] = burnerStarts;
  doc["dhwBurnerStarts"] = dhwBurnerStarts;
  doc["burnerOperationHours"] = burnerOperationHours;

  doc["setPointMax"] = setPointMax;
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
  if (responseStatus != OpenThermResponseStatus::SUCCESS)
  {
    throw responseStatus;
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

void HousenetOpenthermElement::Process()
{

  new_ts = millis();

  if (new_ts - ts > 1000)
  {
    publish("setpoint", "ok");
    ot.setBoilerTemperature(setpoint);

    try
    {
      OTReading thisReading = GetReading();
      publishReading(thisReading);
    }
    catch (OpenThermResponseStatus responseStatus)
    {
       publishu16("ot_response", responseStatus);      
    }

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
/*
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
*/
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

void HousenetOpenthermElement::publishs16(String name, int16_t value) {
  char data[30];
  snprintf(data, 30, "%u", ot.getUInt(value));

  publish(name, data);
}

void HousenetOpenthermElement::publishraw(String name, unsigned long value)
{
   char data[128];
   const uint16_t u88 = value & 0xffff;
   snprintf(data, 128, "%x %lx", u88, value);

  publish(name, data);
}

