# Chocolate Smart Home ESP Connect

A robust ESP device integration library for the [Chocolate Smart Home Backend](https://github.com/ZFudge/chocolate-smart-home-backend) server. This library provides a standardized interface for ESP devices to communicate with the backend server, handling device configuration, MQTT communication, and bidirectional messaging.

## Features

- Simplified MQTT connection management
- Standardized device configuration (MQTT ID, device type, name)
- Built-in WiFi and MQTT status LED indicators
- Bidirectional message handling between server and device
- Easy integration with custom device controllers

## Dependencies

- [PubSubClient](https://github.com/ZFudge/pubsubclient) - MQTT client library for Arduino

## Installation

1. Install the PubSubClient dependency:
```bash
git clone https://github.com/ZFudge/pubsubclient.git
```
2. Copy the library to your Arduino libraries directory

## Usage

Here's a basic example demonstrating how to integrate the library with your ESP device:

```cpp
#include <Chocolate_Smart_Home_ESP_Connect.h>
#include "configuration.h"
#include "custom_controller.h"

CustomController custom_controller;

// Define state retrieval callback
String getCustomControllerState() {
    return DuplexMessenger::getCustomControllerState(&custom_controller);
}

// Define message processing callback
void processMsg(String message) {
    DuplexMessenger::processMsg(message, &custom_controller);
}

void setup() {
    // Configure controller callbacks
    CsmEspConn::controller.setGetState(getCustomControllerState);
    CsmEspConn::controller.setProcessMsgReceived(processMsg);

    // Set device configuration
    CsmEspConn::controller.setMQTTId(MQTT_ID);
    CsmEspConn::controller.setType(CONTROLLER_TYPE);
    CsmEspConn::controller.setWIFILed(LED_WIFI);
    CsmEspConn::controller.setMQTTLed(LED_MQTT);
    CsmEspConn::controller.init(NAME);
    
    // Configure MQTT connection
    CsmEspConn::set_mqtt_server_host_and_port(MQTT_SERVER, MQTT_PORT);
}

void loop() {
    // Handle WiFi connection
    if (!CsmEspConn::wifiIsConnected()) {
        CsmEspConn::connect_WIFI();
    }

    // Handle MQTT connection
    if (!CsmEspConn::mqtt_client.connected()) {
        CsmEspConn::connect_MQTT();
    } else {
        CsmEspConn::mqtt_client.loop();
    }

    // Implement your device's main loop logic here
}
```

## Configuration

Before using the library, ensure you have defined the following configuration variables:

- `MQTT_ID`: Unique identifier for your device
- `CONTROLLER_TYPE`: Type of device controller
- `NAME`: Human-readable device name
- `LED_WIFI`: Pin number for WiFi status LED
- `LED_MQTT`: Pin number for MQTT status LED
- `MQTT_SERVER`: MQTT broker address
- `MQTT_PORT`: MQTT broker port
