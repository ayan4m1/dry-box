# dry-box firmware

## parts

 * [LOLIN D1 Mini](https://www.aliexpress.us/item/2251832342786284.html)
 * [LOLIN SHT30 Shield for D1 Mini](https://www.aliexpress.us/item/2251832575822188.html)
 * **todo:** add STL files for 3D print mount model

## usage

1. Open this directory in VS Code with PlatformIO installed
2. Open a new PlatformIO terminal in VS Code
3. Run `./flash.sh`
4. Follow the prompts

Once each sensor is flashed, they will each create an access point. The password is `humidity`.

Once connected, your browser should open and take you to the config page automatically. If not, go to `http://192.168.4.1`.

Set your WiFi settings, Discord Webhook URL, humidity thresholds, and polling rate. Click Save.

The device will now reboot and should be operational.

**NOTE:** Double-tap the reset button and wait a few seconds for a solid blue LED to get back into the config AP interface later on.