#!/usr/bin/env bash

read -p "How many devices do you want to flash? " DEVICE_COUNT

if [[ ! $DEVICE_COUNT =~ '^[0-9]+$' || $DEVICE_COUNT -lt 1 ]]; then
    echo "Invalid device count!"
    exit 1
fi

if [[ "$DEVICE_COUNT" -eq "1" ]]; then
    read -p "What is the device ID? " DEVICE_ID

    DEVICE_ID="$DEVICE_ID" pio run -t upload
else
    read -p "What is the device ID prefix? " DEVICE_ID_PREFIX

    for i in $(seq 1 $DEVICE_COUNT); do
        read -p "Press ENTER when device ${i} is connected..."
        DEVICE_ID="${DEVICE_ID_PREFIX} ${i}" pio run -t upload
    done
fi