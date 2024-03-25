#ifndef CONTROLLER_DATA_H_
#define CONTROLLER_DATA_H_

struct ControllerData {
  String newDataReceivedTopic;
  String stateRequestedTopic;
  String name;

  String (*getStateMessage)();

  void init(const char* controllerType,
            uint8_t mqttId,
            String name,
            String (*getStateMessage)()
  ) {
    randomSeed(ESP.getCycleCount());

    // Appending a randomly generated integer to the controller name
    // helps the server determine when a reboot has occurred.
    this->name = name + " - " + String(random(0, 999));

    this->stateRequestedTopic  = "/request_device_state/" +
                                 String(mqttId) + "/";
    this->newDataReceivedTopic = "/" + String(controllerType) +
                                 "/" + String(mqttId) + "/";

    this->getStateMessage = getStateMessage;
  }
};

#endif
