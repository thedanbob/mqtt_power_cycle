#ifndef CONFIG_H
#define CONFIG_H
/*
  ====================================================================================================
                        Define constants below or, preferably, in platformio.ini
  ====================================================================================================
*/

//#define WIFI_SSID "wifi_ssid"           // Your WiFi ssid
//#define WIFI_PASS "wifi_password"       // Your WiFi password
//#define WIFI_TIMEOUT 10                 // Minutes to wait for a wifi connection before rebooting

//#define MQTT_SERVER "192.168.0.1"       // Your mqtt server ip address
//#define MQTT_PORT 1883                  // Your mqtt port
//#define MQTT_BASE_TOPIC "sonoff/basic"  // Base mqtt topic
//#define MQTT_USER "sonoff"              // mqtt username
//#define MQTT_PASS "password"            // mqtt password
//#define MQTT_RETAIN true                // Retain availability messages (recommended)
//#define MQTT_QOS 0                      // QOS level for mqtt messages (0 or 1)

//#define NAME "Sonoff Button"            // Home Assistant entity name
//#define DEFAULT_STATE 1                 // Default/initial state of circuit (1 = on)
//#define TOGGLE_DURATION 10              // Seconds to toggle circuit

//#define DISABLE_LINK_LED                // Uncomment to disable link status LED
//#define DEBUG                           // Uncomment to enable debug reporting

/*
  ====================================================================================================
                                         END USER CONFIGURATION
  ====================================================================================================
*/

#include <Arduino.h>

// Defaults
#ifndef WIFI_SSID
  #define WIFI_SSID "wifi_ssid"
#endif
#ifndef WIFI_PASS
  #define WIFI_PASS "wifi_password"
#endif
#ifndef WIFI_TIMEOUT
  #define WIFI_TIMEOUT 10
#endif

#ifndef MQTT_SERVER
  #define MQTT_SERVER "192.168.0.1"
#endif
#ifndef MQTT_PORT
  #define MQTT_PORT 1883
#endif
#ifndef MQTT_BASE_TOPIC
  #define MQTT_BASE_TOPIC "sonoff/basic"
#endif
#ifndef MQTT_USER
  #define MQTT_USER "sonoff"
#endif
#ifndef MQTT_PASS
  #define MQTT_PASS "password"
#endif
#ifndef MQTT_RETAIN
  #define MQTT_RETAIN true
#endif
#ifndef MQTT_QOS
  #define MQTT_QOS 0
#endif

#ifndef NAME
  #define NAME "Sonoff Button"
#endif
#ifndef DEFAULT_STATE
  #define DEFAULT_STATE 1
#endif
#ifndef TOGGLE_DURATION
  #define TOGGLE_DURATION 10
#endif

// Debug helpers
#ifdef DEBUG
  #define LOG_BEGIN(x) Serial.begin(x)
  #define LOG(x) Serial.print(x)
  #define LOG_LN(x) Serial.println(x)
  #define LOG_F(x, ...) Serial.printf(x, __VA_ARGS__)
#else
  #define LOG_BEGIN(x)
  #define LOG(x)
  #define LOG_LN(x)
  #define LOG_F(x, ...)
#endif

// String helpers (must be defined twice for reasons)
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

// GPIO pins
#define LINK_LED 13
#define BUTTON 0
#define RELAY 12

#ifndef DEVICE_MODEL
  #define DEVICE_MODEL "Sonoff Basic"
#endif

#ifndef DEVICE_NAME
  #define DEVICE_NAME DEVICE_MODEL
#endif

#define MQTT_CMD_SUF "/cmd"
#define MQTT_ATTR_SUF "/sys"
#define MQTT_AVTY_SUF "/up"

// Home Assistant defaults
#ifndef MQTT_MSG_PRESS
  #define MQTT_MSG_PRESS "PRESS"
  #define MQTT_MSG_PRESS_DEFAULT
#endif
#ifndef MQTT_MSG_UP
  #define MQTT_MSG_UP "online"
  #define MQTT_MSG_UP_DEFAULT
#endif
#ifndef MQTT_MSG_DOWN
  #define MQTT_MSG_DOWN "offline"
  #define MQTT_MSG_DOWN_DEFAULT
#endif
#ifndef MQTT_MSG_RESTART
  #define MQTT_MSG_RESTART "restart"
#endif

#ifndef VERSION
  #define VERSION "20220412000000"
#endif

#endif
