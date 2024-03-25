#ifndef MQTT_FUNCS_H_
#define MQTT_FUNCS_H_

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "constants.h"
#include "controller_data.h"

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

ControllerData controller;


void publishEspState() {
  const String message = controller.getStateMessage();

  Serial.print("Publishing: ");
  Serial.println(message);
  Serial.print("To topic: ");
  Serial.println(SEND_THIS_CONTROLLERS_DATA_TOPIC);
  Serial.println(
    mqtt_client.publish(
      SEND_THIS_CONTROLLERS_DATA_TOPIC,
      String(message).c_str()
    )
    ? "Success!" : "FAILED!"
  );
}

void handleMqttMsgReceived(
  String topic,
  byte* message,
  unsigned int length,
  void (*processData)(String message)
) {
  /* This is called whenever a message is published to
   * a topic that the ESP8266 is subscribed to. */
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);

  if (topic == controller.stateRequestedTopic ||
      topic == ALL_CONTROLLERS_STATES_REQUESTED_TOPIC)
    return publishEspState();

  String messageTemp;
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
    Serial.print((char)message[i]);
  }
  Serial.println();
  Serial.println(messageTemp);

  processData(messageTemp);

  publishEspState();
}

void init_mqtt_broker(
  void (*handleMqttMsgReceivedImplemented)(String topic, byte* message, unsigned int length),
  const char* mqttServer,
  int mqttPort
) {
  mqtt_client.setServer(mqttServer, mqttPort);
  mqtt_client.setCallback(handleMqttMsgReceivedImplemented);
}

void init_mqtt_broker(
  void (*handleMqttMsgReceivedImplemented)(String topic, byte* message, unsigned int length),
  const char* mqttServer
) {
  mqtt_client.setServer(mqttServer, DEFAULT_MQTT_PORT);
  mqtt_client.setCallback(handleMqttMsgReceivedImplemented);
}

#endif
