#ifndef __HOUSENET_NODE_H__
#define __HOUSENET_NODE_H__

//#include <Arduino.h>

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
#include "sensor/pin/pulsecounter.h"


#include <ArduinoJson.h>
#include "time_utils.h"

#include <queue>

#include "sensor/onewirebus.h"
#include "sensor/pin/pinstate.h"
#include <functional>

#include <WiFiClient.h>

class HousenetElement;

class HousenetNode
{

public:
    HousenetNode();

    void Process();
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
    String station_name;

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

        // Process is called on each element as a part of the main loop.
        virtual void Process(){

        }
        // Override to return a type name.
        virtual String GetType() const = 0;

        virtual void OnMessage(String& topic, String& value) {

        }

        String GetId() { return id; }

        virtual String GetState( String channel );
        virtual void SetState( String channel, String data );

    protected:
      bool publish(const String &topic, const String &payload) const;
      bool publish(const char topic[], const char payload[], bool retained, int qos) const;

    protected:
        HousenetNode* node;
    
    private:        
        const String        id;
};


class HousenetStateElement : public HousenetElement {
    public:
        static const String TYPE;

    public:
        HousenetStateElement(HousenetNode *parent, String id, uint8_t pin);
  
        virtual String GetType() const {
            return TYPE;
        }
        virtual void Process();
        
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
        virtual void Process();
        virtual String GetType() const {
            return TYPE;
        }
    protected:
     
    private:
        OneWireBus bus;
        bool ready;
        TimeUtils timeUtils;
};

class HousenetGpioTriggerElement : public HousenetElement {
    public:

        static const String TYPE;
     
     public:
        HousenetGpioTriggerElement(HousenetNode *parent, String id, uint8_t pin, unsigned long delayMs);
        virtual void OnMessage(String& topic, String& value);
        virtual String GetType() const {
            return TYPE;
        }
        virtual void   SetState( String channel, String data );
        void trigger();

    private:
        uint8_t       m_pin;
        unsigned long m_delay;
};


#endif