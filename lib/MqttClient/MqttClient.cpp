#include "MqttClient.h"
#include <ESP8266WiFi.h>

const String MQTTClient::_commandTopic{MQTT_BASE_TOPIC MQTT_CMD_SUF};
const String MQTTClient::_sysTopic{MQTT_BASE_TOPIC MQTT_ATTR_SUF};
const String MQTTClient::_upTopic{MQTT_BASE_TOPIC MQTT_AVTY_SUF};

MQTTClient::MQTTClient() :
  _wifiClient{},
  _pubSubClient{_wifiClient, MQTT_SERVER, MQTT_PORT},
  _cmdCallback{nullptr}
{
  _pubSubClient.set_callback([this](const MQTT::Publish& pub) {
    String cmd{pub.payload_string()};
    if (cmd == MQTT_MSG_RESTART) ESP.restart();
    if (_cmdCallback == nullptr || cmd != MQTT_MSG_PRESS) return;

    _cmdCallback();
  });
}

bool MQTTClient::connect(String uid) {
  LOG_F("Connecting to MQTT broker %s... ", MQTT_SERVER);
  MQTT::Connect conn{uid};
  conn.set_auth(MQTT_USER, MQTT_PASS)
      .set_will(_upTopic, MQTT_MSG_DOWN, MQTT_QOS, MQTT_RETAIN);

  if (!_pubSubClient.connect(conn)) {
    LOG_LN("FAILED!");
    return false;
  }

  LOG_LN("Subscribing to command topic");
  _pubSubClient.subscribe(_commandTopic, MQTT_QOS);
  _pubSubClient.publish(
    MQTT::Publish{_upTopic, MQTT_MSG_UP}
      .set_retain(MQTT_RETAIN)
      .set_qos(MQTT_QOS)
  );
  return true;
}

void MQTTClient::disconnect() {
  _pubSubClient.publish(
    MQTT::Publish{_upTopic, MQTT_MSG_DOWN}
      .set_retain(MQTT_RETAIN)
      .set_qos(MQTT_QOS)
  );
  _pubSubClient.disconnect();
}

bool MQTTClient::connected() {
  return _pubSubClient.connected();
}

void MQTTClient::loop() {
  _pubSubClient.loop();
}

void MQTTClient::sendSys() {
  String payload{"{"
    "\"IP\":\"" + WiFi.localIP().toString() + "\","
    "\"RSSI\":" + String{WiFi.RSSI()} + ","
    "\"FreeMem\":" + String{ESP.getFreeHeap()} +
  "}"};
  _pubSubClient.publish(_sysTopic, payload);
}

void MQTTClient::sendDiscovery(String uid) {
  String topic{"homeassistant/button/" + uid + "/config"};
  String payload{"{"
    "\"name\":\"" NAME "\","
    "\"uniq_id\":\"" + uid + "\","
    "\"~\":\"" MQTT_BASE_TOPIC "\","
    "\"cmd_t\":\"~" MQTT_CMD_SUF "\","
    "\"json_attr_t\":\"~" MQTT_ATTR_SUF "\","
    "\"avty_t\":\"~" MQTT_AVTY_SUF "\","
    #ifndef MQTT_MSG_PRESS_DEFAULT
    "\"pl_prs\":\"" MQTT_MSG_PRESS "\","
    #endif
    #ifndef MQTT_MSG_UP_DEFAULT
    "\"pl_avail\":\"" MQTT_MSG_UP "\","
    #endif
    #ifndef MQTT_MSG_DOWN_DEFAULT
    "\"pl_not_avail\":\"" MQTT_MSG_DOWN "\","
    #endif
    "\"qos\":" STR(MQTT_QOS) ","
    "\"dev\":{"
      "\"name\":\"" DEVICE_NAME "\","
      "\"mf\":\"ITEAD\","
      "\"mdl\":\"" DEVICE_MODEL "\","
      "\"ids\":[\"" + ESP.getChipId() + "\"],"
      "\"cns\":[[\"mac\",\"" + WiFi.macAddress() + "\"]],"
      "\"sw\":\"" VERSION "\""
    "}"
  "}"};

  _pubSubClient.publish(
    MQTT::Publish{topic, payload}.set_retain()
  );
}

void MQTTClient::setCommandCallback(callback_t cb) {
  _cmdCallback = cb;
}
