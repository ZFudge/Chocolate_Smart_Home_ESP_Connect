#ifndef CONTROLLER_H_
#define CONTROLLER_H_

struct Controller {
    uint8_t mqttId;
    String controllerType;
    String name;

    String newDataReceivedTopic;
    String stateRequestedTopic;

    /* Implemented and set at the CSM controller level. */
    void (*processMsgForController)(String message);
    void setProcessMsgForController(
        void (*processMsgForController)(String message)
    ) {
        this->processMsgForController = processMsgForController;
    }
    /* Implemented and set at the CSM controller level. */
    String (*getControllerState)();
    void setGetControllerState(
        String (*getControllerState)()
    ) {
        this->getControllerState = getControllerState;
    }

    String getControllerConfig() {
        return String(mqttId) + ',' + String(controllerType) + ',' + String(name);
    };

    String getConfigAndState() {
        return getControllerConfig() + ',' + this->getControllerState();
    };

    void setMQTTId(uint8_t mqttId) {this->mqttId = mqttId;}
    void setControllerType(const char* controllerType) {this->controllerType = controllerType;}

    void init(String name) {
        // ESP boot time varies enough for seeding RNG.
        randomSeed(ESP.getCycleCount());
        // Append nonce to controller name to help the server identify when reboots have occurred.
        this->name = name + " - " + String(random(0, 999));
        // Set identity-specific topics.
        this->stateRequestedTopic  = "/request_device_state/" + String(mqttId) + "/";
        this->newDataReceivedTopic = "/" + String(controllerType) + "/" + String(mqttId) + "/";
    }
};

Controller controller_CSM;

#endif
