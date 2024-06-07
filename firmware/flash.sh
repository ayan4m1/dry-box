#!/usr/bin/env bash

is_num='^[0-9]+$'
read_params() {
    read -p "What is the WiFi SSID?" WIFI_SSID
    read -p "What is the WiFi PSK?" WIFI_PSK
    read -p "What is the Discord webhook URL?" DISCORD_WEBHOOK_URL
}

echo "How many devices do you want to flash?"
read DEVICE_COUNT

if ! [[ $DEVICE_COUNT =~ $is_num ]]; then
    echo "Invalid device count!"
    exit 1
fi

if [[ $DEVICE_COUNT -lt 1 ]]; then
    echo "Invalid device count!"
    exit 1
fi

if [[ "$DEVICE_COUNT" -eq "1" ]]; then
    read -p "What is the device ID?" DEVICE_ID
    read_params

    pio run -t upload
else
    read -p "What is the device ID prefix?" DEVICE_ID_PREFIX
    read_params

    for i in $(seq 1 $DEVICE_COUNT); do
        echo "Flashing device ${i}"

        DEVICE_ID="${DEVICE_ID_PREFIX} ${i}"

        pio run -t upload
        read -p "Press enter when next device is connected..."
    done
fi