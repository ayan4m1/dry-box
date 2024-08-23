#define USE_LITTLEFS true
#define DOUBLERESETDETECTOR_DEBUG false
#define USE_DYNAMIC_PARAMETERS true
#define USING_BOARD_NAME false
// set to 3 or 4 to enable info/debug logging respectively
#define _ESP_WM_LITE_LOGLEVEL_ 0
#define REQUIRE_ONE_SET_SSID_PW true
#define USING_MRD false

#include <Arduino.h>
#include <Discord_WebHook.h>
#include <ESP8266mDNS.h>
#include <ESP_WiFiManager_Lite.h>
#include <FastLED.h>
#include <WEMOS_SHT3X.h>

#define WIFI_PSK "drybox123"

#define DISCORD_WARNING_ICON "yellow_circle"
#define DISCORD_ALERT_ICON "red_circle"
#define DISCORD_ERROR_ICON "no_entry_sign"
#define DISCORD_CLEAR_ICON "green_circle"
#define DISCORD_PING_ICON "hourglass"
#define DISCORD_STARTUP_ICON "tada"
#define DISCORD_MSG_BUFFER 256

// start dynamic params
#define DEFAULT_DEVICE_NAME "Dry Box 1"
#define DEFAULT_POLLING_RATE "120"
#define DEFAULT_WEBHOOK_URL ""
#define DEFAULT_ALARM_THRESHOLD "30"
#define DEFAULT_WARNING_THRESHOLD "20"

#define MAX_PERCENTAGE_LEN 3
#define MAX_POLLING_RATE_LEN 5
#define MAX_DEVICE_NAME_LEN 32
#define MAX_DISCORD_WEBHOOK_LEN 192

bool LOAD_DEFAULT_CONFIG_DATA = false;
ESP_WM_LITE_Configuration defaultConfig;

char deviceName[MAX_DEVICE_NAME_LEN + 1] = DEFAULT_DEVICE_NAME;
char sensorPollingRate[MAX_POLLING_RATE_LEN + 1] = DEFAULT_POLLING_RATE;
char discordWebhookUrl[MAX_DISCORD_WEBHOOK_LEN + 1] = DEFAULT_WEBHOOK_URL;
char humidityAlarmThreshold[MAX_PERCENTAGE_LEN + 1] = DEFAULT_ALARM_THRESHOLD;
char humidityWarningThreshold[MAX_PERCENTAGE_LEN + 1] =
    DEFAULT_WARNING_THRESHOLD;

MenuItem myMenuItems[] = {
    {"snn", "Device Name", deviceName, MAX_DEVICE_NAME_LEN},
    {"hwt", "Warning %", humidityWarningThreshold, MAX_PERCENTAGE_LEN},
    {"hat", "Alarm %", humidityAlarmThreshold, MAX_PERCENTAGE_LEN},
    {"dwu", "Discord Webhook", discordWebhookUrl, MAX_DISCORD_WEBHOOK_LEN},
    {"spr", "Poll Rate (s)", sensorPollingRate, MAX_POLLING_RATE_LEN}};

uint16_t NUM_MENU_ITEMS = sizeof(myMenuItems) / sizeof(MenuItem);
// end dynamic params

SHT3X sensor;
float lastHumidity = 0.0f;
bool alarmTripped = false;
Discord_Webhook* webhook = new Discord_Webhook();
ESP_WiFiManager_Lite* manager = new ESP_WiFiManager_Lite();

// start utility functions

bool areFloatsEqual(float a, float b, float epsilon = 0.01f) {
  return fabs(a - b) < epsilon;
}

String camelToSnake(const char* src) {
  String input = String(src);
  String result = "";

  char c = tolower(input[0]);
  result += char(c);

  for (uint16_t i = 1; i < input.length(); i++) {
    c = input[i];

    if (isupper(c)) {
      result += '_';
      result += char(tolower(c));
    } else if (isAlphaNumeric(c)) {
      result += c;
    }
  }

  return result;
}

void sendMessage(const char* message) {
  if (strlen(discordWebhookUrl) <= 0) {
    Serial.println(F("Invalid Discord webhook URL!"));
    return;
  }

  if (!webhook->send(message)) {
    Serial.println(F("Failed to trigger Discord webhook!"));
  }
}

void sendUpdateMessage(const char* icon) {
  const float humidity = sensor.humidity;
  char message[DISCORD_MSG_BUFFER];

  snprintf(message, DISCORD_MSG_BUFFER, ":%s: %s is at %.2f%% humidity", icon,
           deviceName, humidity);

  sendMessage(message);
}

void sendErrorMessage() {
  char message[DISCORD_MSG_BUFFER];

  snprintf(message, DISCORD_MSG_BUFFER, ":%s: %s has a sensor fault!",
           DISCORD_ERROR_ICON, deviceName);
  sendMessage(message);
}

bool pollSensor() {
  auto failed = sensor.get() != 0;

  if (failed) {
    Serial.println(F("Failed to poll sensor!"));
    sendErrorMessage();
    return false;
  }

  return true;
}

void checkSensorState() {
  uint8_t alarmThreshold = atoi(humidityAlarmThreshold);
  uint8_t warningThreshold = atoi(humidityWarningThreshold);

  if (alarmThreshold == 0 || warningThreshold == 0 ||
      warningThreshold > alarmThreshold) {
    Serial.println(F("Invalid thresholds, skipping update!"));
    return;
  }

  if (!pollSensor()) {
    return;
  }

  Serial.printf("Humidity is %.2f%%\n", sensor.humidity);

  auto clamped = roundf(sensor.humidity * 100.0f) / 100.0f;

  if (areFloatsEqual(lastHumidity, clamped)) {
    Serial.println(F("Humidity value has not changed significantly!"));
    return;
  }

  if (clamped >= warningThreshold) {
    if (clamped >= alarmThreshold) {
      Serial.println(F("Alarm tripped"));
      alarmTripped = true;
      sendUpdateMessage(DISCORD_ALERT_ICON);
    } else {
      sendUpdateMessage(DISCORD_WARNING_ICON);
    }
  } else if (alarmTripped) {
    Serial.println(F("Alarm reset"));
    alarmTripped = false;
    sendUpdateMessage(DISCORD_CLEAR_ICON);
  }
}

// end utility functions

void setup() {
  Serial.begin(115200);
  Serial.println();

  // load the dynamic data we need to get the device name
  manager->extLoadDynamicData();
  // Derive hostname from device name
  auto hostname = camelToSnake(deviceName);
  Serial.printf("Device name is %s\n", hostname.c_str());

  // Configure WiFi params if we are going into AP mode
  manager->setConfigPortal(hostname, WIFI_PSK);
  manager->begin(hostname.c_str());

  // Initialize the Discord webhook lib
  webhook->begin(discordWebhookUrl);

  Serial.println(F("Initialized"));

  // Post a startup message if we are set up
  if (!manager->isConfigMode() && pollSensor()) {
    sendUpdateMessage(DISCORD_STARTUP_ICON);
  }
}

void loop() {
  // Must be called once every loop()
  manager->run();

  // Get polling rate from dynamic params
  uint16_t pollingRate = atoi(sensorPollingRate);

  if (pollingRate == 0) {
    Serial.println(
        F("Warning: Polling rate is zero, no updates will be performed!"));
    return;
  } else if (manager->isConfigMode()) {
    return;
  }

  // Sensor polling loop
  EVERY_N_SECONDS(pollingRate) { checkSensorState(); }

  // Device healthcheck loop
  EVERY_N_HOURS(24) {
    if (pollSensor()) {
      sendUpdateMessage(DISCORD_PING_ICON);
    }
  }
}
