
#ifndef CHOCOLATE_SMART_HOME_ESP_CONNECT_H
#define CHOCOLATE_SMART_HOME_ESP_CONNECT_H

// Blacklist platforms using https://github.com/arduino/ArduinoCore-api due to
// incompatibilities.
#if defined(ARDUINO_API_VERSION)
#error Platforms using ArduinoCore-API not supported
#endif

// Version format: xxyyzz == "xx.yy.zz"
#define CHOCOLATE_SMART_HOME_ESP_CONNECT_VERSION 10000
#define CHOCOLATE_SMART_HOME_ESP_CONNECT_VERSION_STRING "1.0.0"

#include "chocolate_smart_home_esp_connect/connect_funcs.h"
#include "chocolate_smart_home_esp_connect/controller.h"
#include "chocolate_smart_home_esp_connect/mqtt_funcs.h"

#endif