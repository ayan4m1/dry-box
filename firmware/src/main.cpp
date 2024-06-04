#include <Arduino.h>
#include <Discord_WebHook.h>
#include <FastLED.h>
#include <WEMOS_SHT3X.h>

#define DEVICE_NAME "Alpha"
#define WIFI_SSID "qux"
#define WIFI_PSK "wi9NNYara"
#define DISCORD_WEBHOOK_URL                               \
  "https://discord.com/api/webhooks/1247389183543218239/" \
  "Duwl0KRroGvqmGav2_KbKhcQP18uYCo80jQtrsyFXe8s7OQ9TGBlGDrCdCjrGUqQ7gbK"

#define MESSAGE_BUFFER 64
#define WARNING_THRESHOLD 40
#define ALARM_THRESHOLD 60
#define POLLING_RATE_MS 60000
#define WARNING_ICON F("warning")
#define ALARM_ICON F("bangbang")
#define CLEAR_ICON F("white_check_mark")
#define ERROR_ICON F("no_entry_sign")

bool alarmTripped;
Discord_Webhook webhook;
SHT3X sensor;

void setup() {
  webhook.begin(DISCORD_WEBHOOK_URL);
  webhook.addWiFi(WIFI_SSID, WIFI_PSK);
  webhook.connectWiFi();
}

void sendMessage(const __FlashStringHelper* icon, const float humidity) {
  char message[MESSAGE_BUFFER];

  if (humidity > 0) {
    sprintf(message, ":%s: %s is at %.2f%% humidity", icon, DEVICE_NAME,
            humidity);
  } else {
    sprintf(message, ":%s: %s is at unknown humidity!", icon, DEVICE_NAME);
  }

  webhook.send(message);
}

bool updateSensor() { return sensor.get() == 0; }

void loop() {
  EVERY_N_MILLIS(POLLING_RATE_MS) {
    if (!updateSensor()) {
      sendMessage(ERROR_ICON, -1);
    }

    if (sensor.humidity > ALARM_THRESHOLD) {
      if (!alarmTripped) {
        alarmTripped = true;
      }
      sendMessage(ALARM_ICON, sensor.humidity);
    } else if (sensor.humidity > WARNING_THRESHOLD) {
      sendMessage(WARNING_ICON, sensor.humidity);
    } else if (alarmTripped) {
      alarmTripped = false;
      sendMessage(CLEAR_ICON, sensor.humidity);
    }
  }
}
