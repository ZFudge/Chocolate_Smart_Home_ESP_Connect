#ifndef CONNECT_FUNCS_H_
#define CONNECT_FUNCS_H_

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "constants.h"
#include "misc_funcs.h"
#include "mqtt_funcs.h"
#include "wifi_configuration.h"


void connect_WIFI_CSM() {
    delay(10);
    Serial.print("Connecting to ");
    Serial.print(SSID);
    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        flash();
        delay(1500);
        Serial.print(".");
    }
    Serial.println();

    for (byte i = 0; i < 2; i++) {
        flash();
        delay(500);
    }

    Serial.print("Connected to WiFi! Address: ");
    Serial.println(WiFi.localIP());
}


void connect_MQTT_CSM() {
    while (!mqtt_client_CSM.connected()) {
        Serial.print("Attempting MQTT connection to ");
        Serial.print(mqtt_client_CSM.domain);
        Serial.print(" on port ");
        Serial.print(mqtt_client_CSM.port);
        Serial.println("...");

        digitalWrite(LED_BUILTIN, LOW);

        if (!mqtt_client_CSM.connect(controller_CSM.name.c_str())) {
            // Print failure and wait 5 seconds before trying connection again.
            Serial.print("failed, rc=");
            Serial.print(mqtt_client_CSM.state());
            Serial.println(" trying again in a few seconds...");
            flash();
            digitalWrite(LED_BUILTIN, HIGH);
            delay(5000);
            continue;
        }

        Serial.println("Connected!");
        for (byte i = 0; i < 5; i++) {
            flash();
            delay(i * 100);
        }

        Serial.print("Subscribing to ");
        Serial.print(controller_CSM.newDataReceivedTopic + ", ");
        Serial.print(controller_CSM.stateRequestedTopic + ", and ");
        Serial.println(ALL_CONTROLLERS_STATES_REQUESTED_TOPIC);
        mqtt_client_CSM.subscribe(ALL_CONTROLLERS_STATES_REQUESTED_TOPIC);
        mqtt_client_CSM.subscribe(controller_CSM.stateRequestedTopic.c_str());
        mqtt_client_CSM.subscribe(controller_CSM.newDataReceivedTopic.c_str());
        Serial.println("Subscribed!");

        Serial.println("Attempting initial publish...");
        publishState();
    }
}

#endif
