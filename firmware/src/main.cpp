#include <Arduino.h>
#include <Discord_WebHook.h>
#include <FastLED.h>
#include <WEMOS_SHT3X.h>

#ifndef DEVICE_ID
#error "DEVICE_ID is required!"
#endif
#ifndef WIFI_SSID
#error "WIFI_SSID is required!"
#endif
#ifndef WIFI_PSK
#error "WIFI_PSK is required!"
#endif
#ifndef DISCORD_WEBHOOK_URL
#error "DISCORD_WEBHOOK_URL is required!"
#endif

#define DISCORD_WARNING_ICON "warning"
#define DISCORD_ALERT_ICON "bangbang"
#define DISCORD_ERROR_ICON "no_entry_sign"
#define DISCORD_CLEAR_ICON "white_check_mark"
#define DISCORD_MSG_BUFFER 64
#define HUM_WARN_THRESH 40
#define HUM_ALARM_THRESH 60
#define POLLING_RATE_MS 60000

bool alarmTripped;
Discord_Webhook webhook;
SHT3X sensor;

void setup() {
  webhook.begin(DISCORD_WEBHOOK_URL);
  webhook.addWiFi(WIFI_SSID, WIFI_PSK);
  webhook.connectWiFi();
}

void sendMessage(const float humidity, const char *icon) {
  char message[DISCORD_MSG_BUFFER];

  if (humidity > 0) {
    sprintf(message, ":%s: %s is at %.2f%% humidity", icon, DEVICE_ID,
            humidity);
  } else {
    sprintf(message, ":%s: %s has a sensor fault!", DISCORD_ERROR_ICON,
            DEVICE_ID);
  }

  webhook.send(message);
}

bool updateSensor() { return sensor.get() == 0; }

void loop() {
  EVERY_N_MILLIS(POLLING_RATE_MS) {
    if (alarmTripped) {
      return;
    }

    if (!updateSensor()) {
      sendMessage(-1, nullptr);
    }

    if (sensor.humidity > HUM_ALARM_THRESH) {
      if (!alarmTripped) {
        alarmTripped = true;
      }
      sendMessage(sensor.humidity, DISCORD_ALERT_ICON);
    } else if (sensor.humidity > HUM_WARN_THRESH) {
      sendMessage(sensor.humidity, DISCORD_WARNING_ICON);
    } else if (alarmTripped) {
      alarmTripped = false;
      sendMessage(sensor.humidity, DISCORD_CLEAR_ICON);
    }
  }
}
