# dry-box firmware

## parts

- [LOLIN D1 Mini](https://www.aliexpress.us/item/2251832342786284.html)
- [LOLIN SHT30 Shield for D1 Mini](https://www.aliexpress.us/item/2251832575822188.html)
- **todo:** add STL files for 3D print mount model

## usage

1. Open this directory in VS Code with PlatformIO installed.
2. Connect the target device via USB.
3. Use the `PlatformIO > Upload` action to flash the firmware.
4. Connect to the WiFi AP `Dry Box 1` with PSK `drybox123`.
5. Your browser should open and take you to the config page automatically. If not, go to `http://192.168.4.1`.
6. Set your WiFi settings, Discord Webhook URL, humidity thresholds, and polling rate. Click Save.
7. The device will now reboot and should be operational.

**NOTE:** Double-tap the reset button and wait a few seconds for a solid blue LED to get back into the config AP interface later on.
