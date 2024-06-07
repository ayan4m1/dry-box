#define USE_LITTLEFS true
#define DOUBLERESETDETECTOR_DEBUG false
#define USE_DYNAMIC_PARAMETERS true
#define USING_BOARD_NAME false
// set to 0 to disable WiFiManager logging
#define _ESP_WM_LITE_LOGLEVEL_ 3
#define REQUIRE_ONE_SET_SSID_PW true
#define USING_MRD false

#include <Arduino.h>
#include <Discord_WebHook.h>
#include <ESP8266mDNS.h>
#include <ESP_WiFiManager_Lite.h>
#include <FastLED.h>
#include <WEMOS_SHT3X.h>

#ifndef DEVICE_ID
#define DEVICE_ID ""
#pragma warning "DEVICE_ID is required!"
#endif

#define WIFI_PSK "humidity"

#define DISCORD_WARNING_ICON "warning"
#define DISCORD_ALERT_ICON "bangbang"
#define DISCORD_ERROR_ICON "no_entry_sign"
#define DISCORD_CLEAR_ICON "white_check_mark"
#define DISCORD_MSG_BUFFER 64

// start dynamic params
#define DEFAULT_POLLING_RATE "120"
#define DEFAULT_WEBHOOK_URL ""
#define DEFAULT_ALARM_THRESHOLD "60"
#define DEFAULT_WARNING_THRESHOLD "40"

#define MAX_PERCENTAGE_LEN 3
#define MAX_POLLING_RATE_LEN 5
#define MAX_DISCORD_WEBHOOK_LEN 192

bool LOAD_DEFAULT_CONFIG_DATA = false;
ESP_WM_LITE_Configuration defaultConfig;

char sensorPollingRate[MAX_POLLING_RATE_LEN + 1] = DEFAULT_POLLING_RATE;
char discordWebhookUrl[MAX_DISCORD_WEBHOOK_LEN + 1] = DEFAULT_WEBHOOK_URL;
char humidityAlarmThreshold[MAX_PERCENTAGE_LEN + 1] = DEFAULT_ALARM_THRESHOLD;
char humidityWarningThreshold[MAX_PERCENTAGE_LEN + 1] =
    DEFAULT_WARNING_THRESHOLD;

MenuItem myMenuItems[] = {
    {"hwt", "Warn %", humidityWarningThreshold, MAX_PERCENTAGE_LEN},
    {"hat", "Alarm %", humidityAlarmThreshold, MAX_PERCENTAGE_LEN},
    {"dwu", "Discord Webhook", discordWebhookUrl, MAX_DISCORD_WEBHOOK_LEN},
    {"spr", "Poll Rate (s)", sensorPollingRate, MAX_POLLING_RATE_LEN}};

uint16_t NUM_MENU_ITEMS = sizeof(myMenuItems) / sizeof(MenuItem);
// end dynamic params

SHT3X sensor;
bool alarmTripped = false;
Discord_Webhook* webhook = new Discord_Webhook();
ESP_WiFiManager_Lite* manager = new ESP_WiFiManager_Lite();

// start utility functions

String camelToSnake(const char* src) {
  String input = String(src);
  String result = "";

  char c = tolower(input[0]);
  result += char(c);

  for (int i = 1; i < input.length(); i++) {
    c = input[i];

    if (isupper(c)) {
      result += '_';
      result += char(tolower(c));
    } else {
      result += c;
    }
  }

  result.replace(" ", "");

  return result;
}

void sendMessage(const float humidity, const char* icon) {
  char message[DISCORD_MSG_BUFFER];

  if (humidity > 0) {
    sprintf(message, ":%s: %s is at %.2f%% humidity", icon, DEVICE_ID,
            humidity);
  } else {
    sprintf(message, ":%s: %s has a sensor fault!", DISCORD_ERROR_ICON,
            DEVICE_ID);
  }

  if (strlen(discordWebhookUrl) > 0) {
    webhook->send(message);
  }
}

void pollSensor() {
  uint8_t alarmThreshold = atoi(humidityAlarmThreshold);
  uint8_t warningThreshold = atoi(humidityWarningThreshold);

  if (alarmThreshold == 0 || warningThreshold == 0) {
    Serial.println(F("Invalid thresholds, skipping poll!"));
    return;
  }

  Serial.println(F("Polling sensor..."));

  if (sensor.get() != 0) {
    sendMessage(-1, nullptr);
    return;
  }

  Serial.printf("%.2f%% humidity\n", sensor.humidity);

  if (sensor.humidity > alarmThreshold) {
    alarmTripped = true;
    sendMessage(sensor.humidity, DISCORD_ALERT_ICON);
  } else if (sensor.humidity > warningThreshold) {
    sendMessage(sensor.humidity, DISCORD_WARNING_ICON);
  } else if (alarmTripped) {
    alarmTripped = false;
    sendMessage(sensor.humidity, DISCORD_CLEAR_ICON);
  }
}

// end utility functions

void setup() {
  Serial.begin(115200);
  Serial.println();

  auto hostname = camelToSnake(DEVICE_ID);
  Serial.printf("Startup using hostname %s\n", hostname.c_str());

  manager->setConfigPortal(hostname, WIFI_PSK);
  manager->begin(hostname.c_str());
  if (strlen(discordWebhookUrl) > 0) {
    webhook->begin(discordWebhookUrl);
  }

  Serial.println(F("Initialized"));

  if (!manager->isConfigMode()) {
    pollSensor();
  }
}

void loop() {
  manager->run();

  uint16_t pollingRate = atoi(sensorPollingRate);

  if (pollingRate == 0 || manager->isConfigMode()) {
    return;
  }

  EVERY_N_SECONDS(pollingRate) { pollSensor(); }
}
