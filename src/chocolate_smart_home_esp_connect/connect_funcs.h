#ifndef CONNECT_FUNCS_H_
#define CONNECT_FUNCS_H_

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <PubSubClient.h>

#include "constants.h"
#include "misc_funcs.h"
#include "mqtt_funcs.h"
#include "wifi_configuration.h"


namespace CsmEspConn {

void connect_WIFI() {
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


void connect_MQTT() {
    while (!mqtt_client.connected()) {
        Serial.print("Attempting MQTT connection to ");
        Serial.print(mqtt_client.domain);
        Serial.print(" on port ");
        Serial.print(mqtt_client.port);
        Serial.println("...");

        digitalWrite(LED_BUILTIN, LOW);

        if (!mqtt_client.connect(CsmEspConn::controller.name.c_str())) {
            // Print failure and wait 5 seconds before trying connection again.
            Serial.print("failed, rc=");
            Serial.print(mqtt_client.state());
            Serial.print(" trying again in a few seconds");
            flash();
            digitalWrite(LED_BUILTIN, HIGH);
            for (byte i = 0; i < 5; i++) {
                Serial.print(".");
                delay(1000);
            }
            Serial.println();
            continue;
        }

        Serial.println("Connected!");
        for (byte i = 0; i < 5; i++) {
            flash();
            delay(i * 100);
        }

        Serial.print("Subscribing to ");
        Serial.print(String(CsmEspConn::controller.newDataReceivedTopic) + ", ");
        Serial.print(String(CsmEspConn::controller.stateRequestedTopic) + ", and ");
        Serial.println(ALL_CONTROLLERS_STATES_REQUESTED_TOPIC);
        mqtt_client.subscribe(ALL_CONTROLLERS_STATES_REQUESTED_TOPIC);
        mqtt_client.subscribe(CsmEspConn::controller.stateRequestedTopic.c_str());
        mqtt_client.subscribe(CsmEspConn::controller.newDataReceivedTopic.c_str());
        Serial.println("Subscribed!");

        Serial.println("Attempting initial publish...");
        publishConfigAndState();
    }
}

}

#endif
