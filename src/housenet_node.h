#ifndef __HOUSENET_NODE_H__
#define __HOUSENET_NODE_H__

#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <MQTT.h>
//#include <ESP8266HTTPClient.h>
#include "emontx/emontx.h"
#include "meter/pulsecounter.h"
#include "meter/pulsemeter.h"

#include <ArduinoJson.h>
#include "time_utils.h"

#include <queue>
#include "opentherm/OpenTherm.h"
#include "sensor/onewirebus.h"
#include "sensor/pinstate.h"
#include <functional>

#include <WiFiClient.h>

class HousenetElement;

class HousenetNode
{

public:
    HousenetNode();

    void process();
    void debug(String data);

protected:

    
    void setupMqtt(String host);
    void connectMqtt();

    void setMeterValue(String name, uint32_t value);

protected:
    bool parse_pulsecounters(const JsonVariant &message);

    void publishStatus();

    String GetStatus();
    void CreateElements(JsonArray array);

    //PulseMeter* getMeterById(String id);
    HousenetElement* FindElement(String& type, String& id);

public: 
    String station_id;

public:
    AsyncWebServer server;

    bool useMqtt;


    MQTTClient client;

    AsyncWebSocket ws, ws_data;
    WiFiClient net;
       
    String ip_addr;

    bool processing_enabled = true;

    AsyncWebSocketClient *data_websocket = nullptr;

    TimeUtils timeUtils;

    std::vector<HousenetElement*> elements;
};

class HousenetElement {
    public:
        HousenetElement(HousenetNode *parent, String id) : node(parent), id(id)
        {
            
        }
        virtual void process(){

        }
        virtual String getType() = 0;

        virtual void OnMessage(String& topic, String& value) {

        }

        String GetId() { return id; }

        virtual String GetState( String channel );
        virtual void SetState( String channel, String data );

    protected:
      bool publish(const String &topic, const String &payload);
      bool publish(const char topic[], const char payload[], bool retained, int qos);

    protected:
        HousenetNode* node;
    
    private:        
        const String        id;
};

class HousenetOpenthermElement : public HousenetElement {
    public:
        static const String TYPE;

    public:
        HousenetOpenthermElement(HousenetNode *parent, String id, uint8_t pinIn, uint8_t pinOut);
        virtual void process();
        virtual String getType() {
            return TYPE;
        }
        virtual String GetState( String channel );
        virtual void   SetState( String channel, String data );
    protected:
    
        unsigned long publish_f88(OpenThermMessageID id, const char* topic);
        int16_t       publish_s16(OpenThermMessageID id, const char* topic); 
        unsigned long publish_u16(OpenThermMessageID id, const char* topic);
        void          publish_raw(OpenThermMessageID id, const char* topic);
        
    private:
        OpenTherm ot;

        unsigned long ts = 0, new_ts = 0; 
        float setpoint = 60;
};


class HousenetMeterElement : public HousenetElement {
    public:
        static const String TYPE;

    public:
        HousenetMeterElement(HousenetNode *parent, String id, uint8_t pin, uint32_t debounce);
        virtual void process();

        virtual String getType() {
            return TYPE;
        }

        virtual void OnMessage(String& topic, String& value);
        
        virtual String GetState( String channel );
        virtual void   SetState( String channel, String data );
    protected:
        void pulse(const PulseMeter *pc);

    private:
        PulseMeter*   pulse_meter = nullptr;
        bool meter_updated        = false;
};

class HousenetEmonTXElement : public HousenetElement {
    public:
        static const String TYPE;

    public:
        HousenetEmonTXElement(HousenetNode *parent, String id);
        virtual void process();
        virtual String getType() {
            return TYPE;
        }
    protected:
        void processEmon(const EmonMessage &msg);
        void publish_emon(const EmonMessage &msg, AsyncWebSocketClient *client);
    
    private:
        EmonTX emontx;
        EmonMessage last_emon_message;
};

class HousenetStateElement : public HousenetElement {
    public:
        static const String TYPE;

    public:
        HousenetStateElement(HousenetNode *parent, String id, uint8_t pin);
  
        virtual String getType() {
            return TYPE;
        }
        virtual void process();
        
        virtual String GetState( String channel );
        void callback(const PinState* state);
    protected:
     
    private:
        PinState pinState;
        bool ready = false;
};

class HousenetOneWireElement : public HousenetElement {
    public:
        static const String TYPE;

    public:
        HousenetOneWireElement(HousenetNode *parent, String id, uint8_t pin);
        virtual void process();
        virtual String getType() {
            return TYPE;
        }
    protected:
     
    private:
        OneWireBus bus;
        bool ready;
        TimeUtils timeUtils;
};


#endif