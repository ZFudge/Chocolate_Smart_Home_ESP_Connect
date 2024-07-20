#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#if defined(ESP32)
#define LED_BUILTIN       2
#endif

#define DEFAULT_MQTT_PORT 1883


const char ALL_CONTROLLERS_STATES_REQUESTED_TOPIC[]  = "/request_devices_state/";
const char SEND_THIS_CONTROLLERS_DATA_TOPIC[]        = "/receive_device_state/";

#endif
