#include <Arduino.h>
#include "housenet_node.h"
#include "debugging.h"
#include <ArduinoJson.h>
#include "AsyncJson.h"
#include "time_utils.h"


//==============================================================================================================
// EMonTX
//==============================================================================================================
HousenetEmonTXElement::HousenetEmonTXElement(HousenetNode *parent, String id) : HousenetElement(parent, id) {
    emontx.onMessage([&](const EmonMessage &msg) {
        Serial.println("onMessage 1");
        processEmon(msg);
        Serial.println("onMessage 2");
        if (node->data_websocket != NULL)
        {
            publish_emon(msg, node->data_websocket);
        }
        Serial.println("onMessage 3");
    });
}

void HousenetEmonTXElement::publish_emon(const EmonMessage &msg, AsyncWebSocketClient *client)
{
    DynamicJsonDocument doc = msg.ToJson();
    String str;
    serializeJson(doc, str);

    client->text(str.c_str());
}

void HousenetEmonTXElement::process() {
    emontx.process();
}

void HousenetEmonTXElement::processEmon(const EmonMessage &msg)
{
    // EMONTX value
    char topic[128];
    char value[64];

    // Always report msg seq as a heartbeat
    sprintf(value, "%d", msg.msg_seq);
    publish("msg", value);

    //node->client.publish("/housenet/" + node->station_id + "/emontx/msg", value);

    // Voltage
    sprintf(value, "%3.3f", msg.v);
    publish("voltage", value);

    // Energy and Power
    for (int k = 0; k < 4; k++)
    {
        if (last_emon_message.msg_seq == 0 || last_emon_message.power[k] != msg.power[k])
        {
            sprintf(topic, "%d/current",  k);
            sprintf(value, "%d", msg.power[k]);
            publish(topic, value);
        }

        if (last_emon_message.msg_seq == 0 || last_emon_message.energy_in[k] != msg.energy_in[k])
        {
            sprintf(topic, "%d/energy/in", k);
            sprintf(value, "%d", msg.energy_in[k]);
            publish(topic, value);
        }

        if (last_emon_message.msg_seq == 0 || last_emon_message.energy_out[k] != msg.energy_out[k])
        {
            sprintf(topic, "%d/energy/out", k);
            sprintf(value, "%d", msg.energy_out[k]);
            publish(topic, value);
        }
    }

    last_emon_message = msg;
}