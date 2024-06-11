#ifndef CONTROLLER_H_
#define CONTROLLER_H_

namespace CsmEspConn {

struct Controller {
    uint8_t mqttId;
    String controllerType;
    String name;

    String newDataReceivedTopic;
    String stateRequestedTopic;

    /* Implemented and set at the CSM controller level. */
    void (*processMsgReceived)(String message);
    void setProcessMsgReceived(
        void (*processMsgReceived)(String message)
    ) {
        this->processMsgReceived = processMsgReceived;
    }
    /* Implemented and set at the CSM controller level. */
    String (*getState)();
    void setGetState(
        String (*getState)()
    ) {
        this->getState = getState;
    }

    String getConfig() {
        return String(mqttId) + ',' + String(controllerType) + ',' + String(name);
    };

    String getConfigAndState() {
        return getConfig() + ',' + this->getState();
    };

    void setMQTTId(uint8_t mqttId) {this->mqttId = mqttId;}
    void setType(const char* controllerType) {this->controllerType = controllerType;}

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

Controller controller;

}

#endif
