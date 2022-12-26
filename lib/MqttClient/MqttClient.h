#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <WiFiClient.h>
#include <PubSubClient.h>
#include <functional>
#include "config.h"

class MQTTClient {
  typedef std::function<void (void)> callback_t;

  public:
    MQTTClient();
    bool connect(String uid);
    void disconnect();
    bool connected();
    void loop();
    void sendSys();
    void sendDiscovery(String uid);
    void setCommandCallback(callback_t cb);

  private:
    static const String _commandTopic;
    static const String _sysTopic;
    static const String _upTopic;

    WiFiClient _wifiClient;
    PubSubClient _pubSubClient;
    callback_t _cmdCallback;
};

#endif
