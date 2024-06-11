#ifndef MQTT_FUNCS_H_
#define MQTT_FUNCS_H_

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "constants.h"
#include "controller.h"


WiFiClient espClient;
PubSubClient mqtt_client_CSM(espClient);


void publishState() {
    const String message = controller_CSM.getConfigAndState();

    Serial.print("Publishing: ");
    Serial.println(message);
    Serial.print("To topic: ");
    Serial.println(SEND_THIS_CONTROLLERS_DATA_TOPIC);
    Serial.println(
        mqtt_client_CSM.publish(
            SEND_THIS_CONTROLLERS_DATA_TOPIC,
            String(message).c_str()
        )
        ? "Success!" : "FAILED!"
  );
}

void msgReceivedCallback(String topic, byte* message, unsigned int length) {
    /* This is called whenever a message is published to
    * a topic that the ESP8266 is subscribed to. */
    Serial.print("Message arrived on topic: ");
    Serial.println(topic);

    if (topic == controller_CSM.stateRequestedTopic ||
        topic == ALL_CONTROLLERS_STATES_REQUESTED_TOPIC)
        return publishState();

    String messageTemp;
    Serial.print("Message: ");
    for (int i = 0; i < length; i++) {
        messageTemp += (char)message[i];
        Serial.print((char)message[i]);
    }
    Serial.println();
    Serial.println(messageTemp);

    controller_CSM.processMsgForController(messageTemp);

    publishState();
}

void set_mqtt_server_host_and_port_CSM(const char* mqttServerHostname, int mqttPort ) {
    mqtt_client_CSM.setServer(mqttServerHostname, mqttPort);
    // PubSubClient.setCallback() throws "mqtt_client_CSM does not name a type" error if
    // .setCallback(), or the outermost function whose scope contains a call to it, does
    // not originate from within the setup() sketch function. Putting here now.
    mqtt_client_CSM.setCallback(msgReceivedCallback);
}

void set_mqtt_server_host_with_default_port_CSM(const char* mqttServer) {
    set_mqtt_server_host_and_port_CSM(mqttServer, DEFAULT_MQTT_PORT);
}


#endif
