#ifndef CONNECT_FUNCS_H_
#define CONNECT_FUNCS_H_

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "constants.h"
#include "misc_funcs.h"
#include "mqtt_funcs.h"
#include "wifi_configuration.h"

void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.print(SSID);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    flash();
    delay(500);
    Serial.print(".");
  }
  for (byte i = 0; i < 5; i++) {
    flash();
    delay(250 - (i * 50));
  }
  Serial.println();
  Serial.print("Connected to WiFi! Address: ");
  Serial.println(WiFi.localIP());
}

void connect_mqtt_broker(int mqttPort, String mqttServer) {
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection on host ");
    Serial.print(mqttServer);
    Serial.print(" on port ");
    Serial.print(mqttPort);
    Serial.println("...");
    digitalWrite(LED_BUILTIN, LOW);
    if (mqtt_client.connect(controller.name.c_str())) {
      Serial.println("Connected!");
      Serial.print("Subscribing to ");
      Serial.print(controller.newDataReceivedTopic + ", ");
      Serial.print(controller.stateRequestedTopic + ", and ");
      Serial.println(ALL_CONTROLLERS_STATES_REQUESTED_TOPIC);
      for (byte i = 0; i < 5; i++) {
        flash();
        delay(i * 100);
      }
      mqtt_client.subscribe(controller.newDataReceivedTopic.c_str());
      mqtt_client.subscribe(controller.stateRequestedTopic.c_str());
      mqtt_client.subscribe(ALL_CONTROLLERS_STATES_REQUESTED_TOPIC);
      Serial.println("Subscribed!");
      Serial.println("Attempting initial publish...");
      publishEspState();
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" trying again in a few seconds...");
      flash();
      digitalWrite(LED_BUILTIN, HIGH);
      // Timeout before trying connection again.
      delay(7500);
    }
  }
}

#endif
