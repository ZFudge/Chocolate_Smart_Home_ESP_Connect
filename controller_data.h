#ifndef CONTROLLER_DATA_H_
#define CONTROLLER_DATA_H_

struct ControllerData {
  String newDataReceivedTopic;
  String stateRequestedTopic;
  String name;

  String (*getStateMessage)();

  void init(const char* controllerType,
            uint8_t mqtt_id,
            String name,
            String (*getStateMessage)()
  ) {
    // Appending a randomly generated integer to the controller name
    // helps the server determine when a reboot has occurred.
    this->name = name + " - " + String(random(0, 999));

    this->stateRequestedTopic  = "/request_device_state/" +
                                 String(mqtt_id) + "/";
    this->newDataReceivedTopic = "/" + String(controllerType) +
                                 "/" + String(mqtt_id) + "/";

    this->getStateMessage = getStateMessage;
  }
};

#endif
