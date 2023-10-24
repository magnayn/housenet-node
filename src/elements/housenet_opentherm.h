#ifndef __HOUSENET_OPENTHERM_H__
#define __HOUSENET_OPENTHERM_H__

//#include <Arduino.h>
#include "housenet_node.h"
#include "opentherm/OpenTherm.h"


class OTReading
{
public:
    void WriteToJson(JsonObject& object) const;

public:
  bool ch;
  bool dhw;
  bool flame;

  // Comes from us
  float setPoint;

  float temperature;

  // Obs:
  unsigned long relModLevel;
  
  float pressure;
  float dhwTemp;
  float tempReturn;
  
  uint16_t burnerStarts;
  uint16_t dhwBurnerStarts;
  uint16_t burnerOperationHours;
  
  float setPointMax;
};

class HousenetOpenthermElement : public HousenetElement {
    public:
        static const String TYPE;

    public:
        HousenetOpenthermElement(HousenetNode *parent, String id, uint8_t pinIn, uint8_t pinOut);
        virtual void Process();
        virtual String GetType() {
            return TYPE;
        }
        virtual String GetState( String channel );
        virtual void   SetState( String channel, String data );

        OTReading GetReading();

    protected:
    
        void publishReading(OTReading& reading);

        int16_t       getS16(OpenThermMessageID id);
        uint16_t      getU16(OpenThermMessageID id);
        float         getF88(OpenThermMessageID id);
        unsigned long getRAW(OpenThermMessageID id);

        /*unsigned long publish_f88(OpenThermMessageID id, const char* topic);
        int16_t       publish_s16(OpenThermMessageID id, const char* topic); 
        unsigned long publish_u16(OpenThermMessageID id, const char* topic);
        void          publish_raw(OpenThermMessageID id, const char* topic);
        */
        
        void publishf(String name, float value);
        void publishu16(String name, uint16_t value);
        void publishs16(String name, int16_t value);
        void publishraw(String name, unsigned long value);
        
    private:
        OpenTherm ot;

        unsigned long ts = 0, new_ts = 0; 
        float setpoint = 60;
};
#endif