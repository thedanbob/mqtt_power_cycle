#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "config.h"
#include "MqttClient.h"
#include "LED.h"
#include "Circuit.h"
#include "Button.h"
#include "Watchdog.h"

MQTTClient mqtt;
LED linkLED;
Circuit circuit;
Button button(&circuit);
Watchdog watchdog;

Ticker sysTimer;
Ticker blinkTimer;
bool updateInProgress{false};
char uid[16];

void setup() {
  sprintf(uid, "sonoff_%06X", ESP.getChipId());

  LOG_BEGIN(115200);
  LOG_F("\n\nUnit ID: %s\n", uid);

  linkLED.begin();
  circuit.begin();
  button.begin(updateInProgress);

  LOG_F("Connecting to wifi %s: ", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.hostname(uid);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  linkLED.blink();
  watchdog.start();
  while (WiFi.waitForConnectResult(1000) != WL_CONNECTED) LOG(".");

  watchdog.stop();
  LOG("done\nIP Address is "); LOG_LN(WiFi.localIP());

  ArduinoOTA.setHostname(uid);

  ArduinoOTA.onStart([]() {
    LOG_LN("OTA update initiated...");

    mqtt.disconnect();
    updateInProgress = true;
  });

  ArduinoOTA.onEnd([]() {
    LOG_LN("\nOTA update done, restarting...");
  });

  ArduinoOTA.onProgress([](auto progress, auto total) {
    LOG_F("Progress: %u%%\r", (progress / (total / 100)));
    linkLED.set((progress / (total / 20)) % 2); // Toggle LED every 5%
  });

  ArduinoOTA.onError([](auto error) {
    LOG_F("OTA error: [%u] ", error);
    switch(error) {
      case OTA_AUTH_ERROR:
        LOG_LN("Auth Failed"); break;
      case OTA_BEGIN_ERROR:
        LOG_LN("Begin Failed"); break;
      case OTA_CONNECT_ERROR:
        LOG_LN("Connect Failed"); break;
      case OTA_RECEIVE_ERROR:
        LOG_LN("Receive Failed"); break;
      case OTA_END_ERROR:
        LOG_LN("End Failed"); break;
    }

    ESP.restart();
  });

  ArduinoOTA.begin(false);
  if (updateInProgress) return; // Skip rest of setup if updating

  // Set callback to run when mqtt command received
  mqtt.setCommandCallback([]() {
    circuit.toggle();
  });

  // Update system info every 10 seconds
  sysTimer.attach(10, []() {
    mqtt.sendSys();
  });
}

void loop() {
  if (!WiFi.isConnected()) {
    linkLED.blink();
    watchdog.start();

    if (WiFi.waitForConnectResult() != WL_CONNECTED) return;
  }

  watchdog.stop();

  ArduinoOTA.handle();
  if (updateInProgress) return; // Disable mqtt handling during update

  // (Re)connect to mqtt
  if (!mqtt.connected()) {
    linkLED.blink();

    if (!mqtt.connect(uid)) {
      delay(100);
      return;
    }

    // Send all info on (re)connection
    mqtt.sendSys();
    mqtt.sendDiscovery(uid);
  }

  linkLED.reset();
  mqtt.loop();
}
