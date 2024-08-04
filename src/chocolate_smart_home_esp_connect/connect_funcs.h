#ifndef CONNECT_FUNCS_H_
#define CONNECT_FUNCS_H_

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <PubSubClient.h>

#include "constants.h"
#include "mqtt_funcs.h"
#include "wifi_configuration.h"


namespace CsmEspConn {

// Wait 5 seconds before trying MQTT connection again.
const int WIFI_CONNECT_RETRY_TIME = 5000;
long lastWifiConnectionAttempt;
bool initialConnection = true;
bool wifiIsConnected() {
    return WiFi.status() == WL_CONNECTED;
}
void connect_WIFI(const byte LED_WIFI) {
    // Defer Wifi connection attempt until 5 seconds have lapsed since last
    // connection attempt.
    if (!initialConnection && millis() - lastWifiConnectionAttempt < WIFI_CONNECT_RETRY_TIME)
        return;

    initialConnection = false;
    Serial.print("Connecting to ");
    Serial.println(SSID);
    WiFi.begin(SSID, PASSWORD);

    digitalWrite(LED_WIFI, HIGH);
    delay(10);
    digitalWrite(LED_WIFI, LOW);
    delay(10);
    digitalWrite(LED_WIFI, HIGH);

    if (!wifiIsConnected()) {
        Serial.print("Failed: ");
        Serial.println(WiFi.status());
        lastWifiConnectionAttempt = millis();
        digitalWrite(LED_WIFI, LOW);
        return;
    }
 
    digitalWrite(LED_WIFI, LOW);
    Serial.print("Connected to WiFi: ");
    Serial.println(WiFi.localIP());
}

// Wait 5 seconds before trying MQTT connection again.
const int MQTT_CONNECT_RETRY_TIME = 5000;
long lastMqttConnectionAttempt;

void connect_MQTT(const byte LED_MQTT) {
    // Defer MQTT connection attempt until 5 seconds have lapsed since last
    // connection attempt.
    if (millis() - lastMqttConnectionAttempt < MQTT_CONNECT_RETRY_TIME)
        return;

    Serial.print("Attempting MQTT connection to ");
    Serial.print(mqtt_client.domain);
    Serial.print(" on port ");
    Serial.print(mqtt_client.port);
    Serial.println("...");
    digitalWrite(LED_MQTT, HIGH);

    if (!mqtt_client.connect(CsmEspConn::controller.name.c_str())) {
        // Print failure and initiate 5 seconds wait before attempting
        // connection again.
        Serial.print("failed, rc=");
        Serial.print(mqtt_client.state());
        Serial.println(" trying again in 5 seconds");
        lastMqttConnectionAttempt = millis();
        return;
    }

    Serial.println("Connected to MQTT!");

    Serial.print("Subscribing to ");
    Serial.print(String(CsmEspConn::controller.newDataReceivedTopic) + ", ");
    Serial.print(String(CsmEspConn::controller.stateRequestedTopic) + ", and ");
    Serial.println(BROADCAST_TOPIC_ALL_CONTROLLER_STATES_REQUESTED);
    mqtt_client.subscribe(BROADCAST_TOPIC_ALL_CONTROLLER_STATES_REQUESTED);
    mqtt_client.subscribe(CsmEspConn::controller.stateRequestedTopic.c_str());
    mqtt_client.subscribe(CsmEspConn::controller.newDataReceivedTopic.c_str());
    Serial.println("Subscribed!");

    Serial.println("Attempting initial publish...");
    publishConfigAndState();
    digitalWrite(LED_MQTT, LOW);
}

}

#endif
