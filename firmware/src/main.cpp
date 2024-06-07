#ifndef DEVICE_ID
#define DEVICE_ID ""
#pragma warning "DEVICE_ID is required!"
#endif

#define USE_LITTLEFS true
#define DOUBLERESETDETECTOR_DEBUG false
#define USE_DYNAMIC_PARAMETERS true
#define USING_BOARD_NAME false

#define MAX_PERCENTAGE_LEN 3
#define MAX_POLLING_RATE_LEN 5
#define MAX_DISCORD_WEBHOOK_LEN 192

char sensorPollingRate[MAX_POLLING_RATE_LEN + 1] = "120";
char discordWebhookUrl[MAX_DISCORD_WEBHOOK_LEN + 1] = "";
char humidityAlarmThreshold[MAX_PERCENTAGE_LEN + 1] = "60";
char humidityWarningThreshold[MAX_PERCENTAGE_LEN + 1] = "40";

MenuItem menuItems[] = {
    {"hwt", "Warn %", humidityWarningThreshold, MAX_PERCENTAGE_LEN},
    {"hat", "Alarm %", humidityAlarmThreshold, MAX_PERCENTAGE_LEN},
    {"dwu", "Webhook URL", discordWebhookUrl, MAX_DISCORD_WEBHOOK_LEN},
    {"spr", "Polling Rate", sensorPollingRate, MAX_POLLING_RATE_LEN}};

uint16_t NUM_MENU_ITEMS = sizeof(menuItems) / sizeof(MenuItem);

#include <Arduino.h>
#include <Discord_WebHook.h>
#include <ESP8266mDNS.h>
#include <ESP_WiFiManager_Lite.h>
#include <FastLED.h>
#include <WEMOS_SHT3X.h>

#define DISCORD_WARNING_ICON "warning"
#define DISCORD_ALERT_ICON "bangbang"
#define DISCORD_ERROR_ICON "no_entry_sign"
#define DISCORD_CLEAR_ICON "white_check_mark"
#define DISCORD_MSG_BUFFER 64

SHT3X sensor;
bool alarmTripped;
Discord_Webhook* webhook = new Discord_Webhook();
ESP_WiFiManager_Lite* manager = new ESP_WiFiManager_Lite();

String camelToSnake(String input) {
  String result = "";

  auto c = tolower(input[0]);
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

  return result;
}

void setup() {
  manager->begin(camelToSnake(DEVICE_ID).c_str());
  if (strlen(discordWebhookUrl) > 0) {
    webhook->begin(discordWebhookUrl);
  }
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

void loop() {
  uint16_t pollingRate = atoi(sensorPollingRate);
  uint8_t alarmThreshold = atoi(humidityAlarmThreshold);
  uint8_t warningThreshold = atoi(humidityWarningThreshold);

  EVERY_N_MILLIS(pollingRate * 1e3) {
    if (alarmTripped) {
      return;
    }

    if (sensor.get() == 0) {
      sendMessage(-1, nullptr);
    }

    if (sensor.humidity > alarmThreshold) {
      alarmTripped = true;
      sendMessage(sensor.humidity, DISCORD_ALERT_ICON);
    } else if (sensor.humidity > warningThreshold) {
      sendMessage(sensor.humidity, DISCORD_WARNING_ICON);
    } else {
      alarmTripped = false;
      sendMessage(sensor.humidity, DISCORD_CLEAR_ICON);
    }
  }
}
