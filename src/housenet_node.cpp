#include <Arduino.h>
#include "platform.h"
#include "housenet_node.h"
#include "elements/housenet_opentherm.h"
#include "elements/housenet_emontx.h"
#include "elements/housenet_meter.h"
#include "debugging.h"
#include <ArduinoJson.h>
#include "AsyncJson.h"
#include "time_utils.h"

//#include <ESP8266WiFi.h>

const char compile_date[] = __DATE__ " " __TIME__;

#ifndef GIT_REV
#define GIT_REV "none"
#endif

const char git_sha1[] = GIT_REV;

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

// Namespace:
// MQTT:
//   /housenet/[mac-id]/[type]/[id] (and subdirectories)

void HousenetNode::debug(String data)
{
    if (useMqtt && client.connected())
    {
        client.publish("/housenet/" + station_id + "/DEBUG", data);
    }
    Serial.println(data);
}

// bool HousenetNode::parse_pulsecounters(const JsonVariant &documentRoot)
// {
//     for (int i = 0; i < pulse_meter_count; i++)
//     {
//         String id = pulse_meter[i].counter.id;

//         if (documentRoot.containsKey(id))
//         {
//             pulse_meter[i].meter.initialize( documentRoot[id] );
//         }
//     }

//     return true;
// }

HousenetNode::HousenetNode() : server(80),
                               client(512),
                               ws("/ws"),
                               ws_data("/data")
{

    // #ifdef HOUSENET_NODE_GAS
    //     pulse_meter_count = 1;
    //     pulse_meter = new PulseMeter[1] {PulseMeter("gas", D4, 400)};
    // #else
    //     //pulse_meter_count = 3;
    //     //pulse_meter = new PulseMeter[3] {PulseMeter("solar", D1, 100), PulseMeter("electric", D2, 100), PulseMeter("flat", D4, 100)};
    // #endif

    // ESP8266? Serial.swap();
    // Serial.begin(115200);

    Serial.println("Bonjourno ESP");
    Serial.println(GetStatus());

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    // Serve files in directory "/www/" when request url starts with "/"
    // Request to the root or none existing files will try to server the defualt
    // file name "index.htm" if exists
    // TODO

    // server.serveStatic("/", LITTLEFS, "/www/")
    //       .setDefaultFile("index.html");

    // server.serveStatic("/", LittleFS, "/www/").setDefaultFile("index.html");
    /*
        server.on("/housenet/pulse", HTTP_GET, [&](AsyncWebServerRequest *request) {
            AsyncResponseStream *response = request->beginResponseStream("application/json");
            DynamicJsonDocument doc(512);

            for (int i = 0; i < pulse_meter_count; i++)
            {
                PulseMeter& meter = pulse_meter[i];

                doc[meter.meter.id]["pulse"] = meter.counter.value;
                doc[meter.meter.id]["meter"] = meter.meter.value;

            }
            serializeJson(doc, *response);

            request->send(response);
        });

        server.on("/housenet/pulse/set", HTTP_PUT, [&](AsyncWebServerRequest *request) {
            for (int i = 0; i < pulse_meter_count; i++)
            {
                // /housenet/pulse/set?solar=1234
                if (request->hasParam(pulse_meter[i].meter.id))
                {
                    String message = request->getParam(pulse_meter[i].meter.id)->value();
                    pulse_meter[i].meter.setValue( atoi(message.c_str()) );
                }
            }
        });

            AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/housenet/pulse", [&](AsyncWebServerRequest *request, JsonVariant &json) {
                parse_pulsecounters(json);
            });


        // put, post, patch
        server.addHandler(handler);
        */

       server.on("/debug", HTTP_GET, [&](AsyncWebServerRequest *request)
        {
            Serial.println("Debug");
        });

server.on("/reset", HTTP_POST, [&](AsyncWebServerRequest *request)
              {
        ESP.restart();
        
        });

    server.on("/housenet/config", HTTP_GET, [&](AsyncWebServerRequest *request)
              {
        Serial.println("Get Config");
        
        Preferences preferences;
        preferences.begin("node", false);
        String config = preferences.getString("config", "[]"); 
        request->send(200, "text/json", config); });

    server.on(
        "/housenet/config", HTTP_POST,
        [&](AsyncWebServerRequest *request)
        {
            Serial.println("set Config");
            // request->send(200, "text/json", "");
        },
        NULL,
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
        {
            // static String config;
            Serial.println("set Config++");

            Preferences preferences;
            preferences.begin("node", false);

            // POSTED DATA
            char *copied_data = new char[len + 1];

            memcpy(copied_data, data, len);
            copied_data[len] = '\0';

            String theConfig(copied_data);

            preferences.putString("config", theConfig);
            delete copied_data;

            preferences.end();

            request->send(200, PSTR("application/json"), "{\"status\": \"successed\",\"message\": \"message is received\"}");
        });

    server.on("/housenet/status", HTTP_GET, [&](AsyncWebServerRequest *request)
              {
        String status = GetStatus();
        request->send(200, "text/json", status); });

    server.on("/housenet/toggle_processing", HTTP_GET, [&](AsyncWebServerRequest *request)
              {
        // TODO? Serial.swap();
        
        processing_enabled = !processing_enabled;
        // Shut everything down
        // for(int i=0;i<pulse_meter_count;i++) {
        //     pulse_meter[i].counter.disable();
        // }
        Serial.end();

        request->send(200, "text/json", processing_enabled ? "on" : "off"); });

    server.on("/housenet/elements", HTTP_GET, [&](AsyncWebServerRequest *request)
              {
        
      
        String url = request->url();
        Serial.println(url);

        if(url.startsWith("/housenet/elements/") )
        {
            // Process a particular sub-element only for the GET
            String rest = url.substring(19);
            int path = rest.indexOf('/');
            int path1 = rest.indexOf('/',path+1);

            String type = rest.substring(0,path);
            String id = rest.substring(path+1,path1-1);
            
            String rData = rest.substring(path1+1);

            HousenetElement * matchedelement = FindElement(type, id);
            
            if( matchedelement == nullptr ) {
                request->send(404, "text/plain", "Not found");
                return;
            }

            request->send(200, "text/json", matchedelement->GetState(rData));
        } else {        
            // Get a list of elements        
            DynamicJsonDocument doc(1024);
            JsonArray array = doc.to<JsonArray>();

            for(auto const& element:  elements) {

                JsonObject obj1 = array.createNestedObject();

                String type = element->GetType();
                String id   = element->GetId();
            
                obj1["type"] = type;
                obj1["id"]   = id;

            }
            
            String data;
            serializeJson(doc, data);
            
            request->send(200, "text/json", data);
        } });

    server.on(
        "/housenet/elements", HTTP_POST,
        [&](AsyncWebServerRequest *request) {

        },
        NULL,
        [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
        {
            String url = request->url();

            // /housenet/elements/{type}/{id}
            if (url.startsWith("/housenet/elements/"))
            {
                String rest = url.substring(19);
                int path = rest.indexOf('/');
                int path1 = rest.indexOf('/', path + 1);

                String type = rest.substring(0, path);
                String id = rest.substring(path + 1, path1 - 1);

                String rData = rest.substring(path1 + 1);

                HousenetElement *matchedelement = FindElement(type, id);

                if (matchedelement == nullptr)
                {
                    request->send(404, "text/plain", "Not found");
                    return;
                }

                if (request->method() == HTTP_POST)
                {

                    // POSTED DATA
                    char *copied_data = new char[len + 1];

                    memcpy(copied_data, data, len);
                    copied_data[len] = '\0';

                    String postedData(copied_data);

                    matchedelement->SetState(rData, postedData);
                    delete copied_data;
                    request->send(200, "text/json", "OK");
                }
            }
            request->send(404, "text/plain", "Not found");
        });

    server.serveStatic("/", LittleFS, "/www/").setDefaultFile("index.html");

    ws.onEvent([&](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
               {
        if (type == WS_EVT_CONNECT)
        {
            client->text("Hello from ESP32 Server");
            Debugging.websocket = client;
        }
        if (type == WS_EVT_DISCONNECT)
        {
            Debugging.websocket = NULL;
        } });

    ws_data.onEvent([&](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
                    {
        if (type == WS_EVT_CONNECT)
        {
            data_websocket = client;
        }
        if (type == WS_EVT_DISCONNECT)
        {
            data_websocket = NULL;
        } });

    server.addHandler(&ws);
    server.addHandler(&ws_data);

    debug("Restart");

    // e.g:
    // "[ { 'type': 'state', id:'main', 'pin': 4 }, "
    //    "{ 'type': 'meter', id:'leccy', 'pin': 16, 'debounce': 100 }"
    //    "]"
    Preferences preferences;
    preferences.begin("node", false);
    String configJson = preferences.getString("config", "{}");

    debug("config: ");
    debug(configJson);

    DynamicJsonDocument doc(1024);
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, configJson);

    useMqtt = false;


    // Test if parsing succeeds.
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
    }
    else
    {
        JsonObject obj = doc.as<JsonObject>();
        String str = obj["mqtt"];
        JsonArray array = obj["elements"].as<JsonArray>();

        if (str != "")
        {
            setupMqtt(str);
            useMqtt = true;
        }

        String name = obj["name"];
        station_name = name;

        // create the elements
        CreateElements(array);
    }

    // Start the servers
    server.begin();
    
    connectMqtt();

    publishStatus();

}

void HousenetNode::CreateElements(JsonArray array)
{
    Serial.println("Creating elements");

    for (JsonVariant v : array)
    {
        JsonObject obj = v.as<JsonObject>();

        String id = obj["id"];
        String type = obj["type"];

        Serial.println("Create type " + type + " id " + id);

        HousenetElement *element = nullptr;

        if (type.equalsIgnoreCase(HousenetStateElement::TYPE))
        {
            int pin = obj.getMember("pin").as<int>();

            element = new HousenetStateElement(this, id, pin);
        }
        else if (type.equalsIgnoreCase(HousenetOpenthermElement::TYPE))
        {
            element = new HousenetOpenthermElement(this, id, obj.getMember("pinIn").as<int>(), obj.getMember("pinOut").as<int>());
        }
        else if (type.equalsIgnoreCase(HousenetMeterElement::TYPE))
        {
            String modeString = obj["mode"] | "CHANGE";
            bool pullup = obj["pullup"] | true;

            int mode = CHANGE;

            if( modeString.equalsIgnoreCase("rising") ) {
                mode = RISING;   
                Serial.println("rising");             
            } else if( modeString.equalsIgnoreCase("falling") ) {
                mode = FALLING;          
                Serial.println("falling");      
            }

            Serial.println(pullup?"pullup":"nopull");

            element = new HousenetMeterElement(this, id, obj.getMember("pin").as<int>(), obj.getMember("debounce").as<int>(), mode, pullup);
        }
        else if (type.equalsIgnoreCase(HousenetEmonTXElement::TYPE))
        {
            element = new HousenetEmonTXElement(this, id);
        }
        else if (type.equalsIgnoreCase(HousenetOneWireElement::TYPE))
        {
            element = new HousenetOneWireElement(this, id, obj.getMember("pin").as<int>());
        }  
        else if (type.equalsIgnoreCase(HousenetGpioTriggerElement::TYPE))
        {
            unsigned long delay = 300;
            if( obj.containsKey("delay") ) 
                delay = obj.getMember("delay").as<int>();

            element = new HousenetGpioTriggerElement(this, id, obj.getMember("pin").as<int>(), delay);
        }

        if (element != nullptr)
        {
            elements.push_back(element);
        }
        else
        {
            Serial.println("ERROR - nothing made.");
        }
    }
}

HousenetElement *HousenetNode::FindElement(String &type, String &id)
{
    //Serial.println("Find Element " + type + " id " + id);

    for (auto const &element : elements)
    {
        if (element->GetType().equalsIgnoreCase(type) && element->GetId().equalsIgnoreCase(id))
            return element;
    }

    // Serial.println("Fail");
    return nullptr;
}

String HousenetNode::GetStatus()
{
    DynamicJsonDocument doc(256);

    uint32_t heap = ESP.getFreeHeap();

    IPAddress ipa = WiFi.localIP();
    String ip_addr = ipa.toString();
    // debug(ip_addr);

    doc["ip"] = ip_addr;
    doc["version"] = VERSION_NUMBER;
    doc["build"] = compile_date;
    doc["sha1"] = git_sha1;
    doc["free"] = heap;

    doc["processing"] = processing_enabled;
    doc["uptime_mins"] = (uint32_t)(timeUtils.getTime() / 60000);

    doc["station_id"] = station_id;
    doc["station_name"] = station_name;
    
    doc["useMqtt"] = useMqtt;    

    doc["rc"] = esp_reset_reason();


    String data;
    serializeJson(doc, data);
    return data;
}

void messageReceived(String &topic, String &payload)
{
    Serial.println("incoming: " + topic + " - " + payload);

    // Note: Do not use the client in the callback to publish, subscribe or
    // unsubscribe as it may cause deadlocks when other things arrive while
    // sending and receiving acknowledgments. Instead, change a global variable,
    // or push to a queue and handle it in the loop after calling `client.loop()`.
}

void HousenetNode::setupMqtt(String host)
{

    Serial.println("Debug: connect MQTT @ " + host);

    station_id = WiFi.macAddress();
    station_id.replace(':', '-');
    
  
    client.onMessage([&](String &theTopic, String &value) -> void
                     {
                         Serial.println("Incoming " + theTopic);

                         

                         char topic[128];
                         sprintf(topic, "%s", theTopic.c_str());

                         // Format incoming: /housenet/24-6F-28-AA-B4-D0/state/main/value

                         char *housenet = strtok(topic + 1, "/");
                         if (housenet == nullptr)
                             return;

                         char *node = strtok(NULL, "/");
                         if (node == nullptr)
                             return;

                        String theNode(node);
                        if( theNode.equalsIgnoreCase("info") )
                        {
                            Serial.println("NodeInfo");
                            return;
                        }

                         const char *type = strtok(NULL, "/");
                         if (type == nullptr)
                             return;

                         const char *id = strtok(NULL, "/");
                         if (id == nullptr)
                             return;

                         const char *rest = strtok(NULL, "");
                         if( rest == nullptr )
                            return;

                         String theType(type);
                         String theId(id);
                         String theRest(rest);

                         auto item = FindElement(theType, theId);

                         if (item == nullptr)
                             return;

                         item->OnMessage(theRest, value);
                         
                     });

    client.begin(host.c_str(), net);
    useMqtt = true;
}

void HousenetNode::connectMqtt()
{
    if( !useMqtt )
        return;

    Serial.println(station_id);

    if (client.connect(station_id.c_str()))
    {
        // publish data
        char topic[128];

        sprintf(topic, "/housenet/%s/node_info/stage", station_id.c_str());
        client.publish(topic, "starting");

        sprintf(topic, "/housenet/%s/node_info/ip", station_id.c_str());
        IPAddress ipa = WiFi.localIP();
        ip_addr = ipa.toString();

        client.publish(topic, ip_addr);

        client.subscribe("/housenet/" + station_id + "/#");

        // Sub to announcements
        client.subscribe("/housenet/info/#");
    }
    
}

void HousenetNode::Process()
{
   /* static int i=0;
    static uint64_t t0 = 0;*/


    uint64_t when = timeUtils.getTime();
/*
    i++;
    if( i == 10000 ) {
        Serial.printf("time for 10000 iterations %ju\n", when - t0);
        i = 0;
        t0 = when;
    }
*/

    if (processing_enabled)
    {
        client.loop();

        if (useMqtt && !client.connected()) {
            Serial.println("MQTT not connected");
            connectMqtt();
        }
        else
        {
            static uint64_t lastUpdate = 0;
            

            if (when > lastUpdate + 30000) // every 30 mins
            {
                Serial.println("Publish Status");
                publishStatus();

                lastUpdate = when;
            }
        }

        for (auto const &element : elements)
        {

            element->Process();
        }
    }
}

void HousenetNode::publishStatus()
{
    char topic[128];

    sprintf(topic, "/housenet/info/%s", station_id.c_str());
    String theStatus = GetStatus();
    if( useMqtt ) {
        client.publish(topic, theStatus);
    }

}
