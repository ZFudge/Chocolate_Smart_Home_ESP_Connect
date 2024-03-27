#ifndef CONTROLLER_DATA_H_
#define CONTROLLER_DATA_H_

struct ControllerData {
  uint8_t mqttId;
  String controllerType;
  String name;

  String newDataReceivedTopic;
  String stateRequestedTopic;

  String getStateMessageBase() {
    return String(this->mqttId) + ',' +
           String(this->controllerType) + ',' +
           String(this->name);
  };

  String (*getStateMessageTypeImplementation)();

  String getStateMessage() {
    return getStateMessageBase() + ',' +
     this->getStateMessageTypeImplementation();
  };

  void init(uint8_t mqttId,
            const char* controllerType,
            String name,
            String (*getStateMessageTypeImplementation)()
  ) {
    this->mqttId = mqttId;
    this->controllerType = controllerType;

    randomSeed(ESP.getCycleCount());
    // Appending a randomly generated integer to the controller name
    // helps the server determine when a reboot has occurred.
    this->name = name + " - " + String(random(0, 999));

    this->getStateMessageTypeImplementation = getStateMessageTypeImplementation;

    this->stateRequestedTopic  = "/request_device_state/" +
                                 String(mqttId) + "/";
    this->newDataReceivedTopic = "/" + String(controllerType) +
                                 "/" + String(mqttId) + "/";

  }
};

#endif
