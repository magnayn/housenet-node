//#include <Arduino.h>
#include "platform.h"
#include "housenet_node.h"
#include <AsyncElegantOTA.h>

#include "housenet_wifi.h"

HousenetNode *node;



void setup()
{
    Serial.begin(115200);
    Serial.println("Setup.. ");

    if (LittleFS.begin())
    {
        Serial.println("LittleFS OK");
        Serial.println(
            LittleFS.exists("/www/index.html") ? "exists" : "not");
    }
    else
    {
        Serial.println("LittleFS NOT OK");
    }

    

Serial.println("wifi_setup..");
    wifi_setup();
Serial.println("wifi_setup done..");


    node = new HousenetNode();
    AsyncElegantOTA.begin(&node->server); // Start ElegantOTA
}

void loop()
{
    wifi_check_status();

    node->Process();

}