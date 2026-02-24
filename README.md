# My Filament Dry Box Adventure

I'm not the first person to do this, but I wanted to document my specific method in case others find it useful.

I wanted a setup that could keep at least 20 spools of filament in a low-humidity (5-15%) environment.

The Sterilite 20 quart gasket box was chosen because it is popular for this purpose and fits my basic needs - it fits four 1kg spools of filament with minimal wasted space and the lid is gasketed to help keep moisture out.

I found a [spool holder design](https://www.printables.com/model/139303-sterilite-20-qt-gasket-box-spool-rack) on Printables by RCNet that uses 3/8" aluminum tubing, which can be bought in a coil and straightened, then cut to length and mounted between the endcaps to let four typical-size 1kg filament spools rest vertically in the box.

3A Molecular Sieve was chosen as the dessicant because it removes more water, faster than silica gel. It does take more heat/time to dry it back out than silica gel, but this should not be an issue if you have enough molecular sieve to rotate it.

I also wanted warnings/alarms for when the sieve granules needed changing, so the LOLIN D1 Mini was chosen as the microcontroller. They are cheap ($3.70 as of this writing), small, and fairly well designed.

**Energy Use Tangent:** The current draw of each ESP8266 in this application is estimated at 70mA based on [this research](https://www.ondrovo.com/a/20170207-esp-consumption/). That's 67mA continuously with "periodic spikes" which add up to an average of 3mA. This means that every six boxes consumes 420mA, or 50 Wh of energy a day (about $0.14/mo at my electricity prices) - assuming I make no optimizations whatsoever for the device to sleep in between polling periods. My television consumes ~260 Wh in standby mode every day. I could likely drop the average current for six sensors to 100mA with sleep optimizations. They would then consume 22 times less power than my television.

There is an SHT30 shield for the D1 mini, which provides our humidity sensing and is simple to socket onto the D1 mini. The SHT30 is a fairly accurate sensor (±2% RH) which will suffice for these purposes.

The firmware uses [ESP_WiFiManager_Lite](https://github.com/khoih-prog/ESP_WiFiManager_Lite) to simplify setup and configuration of the sensors. On first flashing, the sensor will go into Access Point mode with an SSID of `dry_box_1` and the password `drybox123`. Connect to it, then navigate to `http://192.168.4.1` in your browser. Set the WiFi credentials, device name, polling rate, humidity thresholds, and Discord webhook, then click Save. The device will reboot and start functioning. If you need to change configuration, "double-tap" the reset button on the microcontroller and wait a few seconds for a solid blue LED. This means that it is in Access Point mode again.

## Bill of Materials

### [LOLIN D1 Mini V4](https://www.aliexpress.us/item/3256806460241404.html)

The link above is the official LOLIN Aliexpress store. If it becomes unavailable, you can find clones of it by searching ["d1 mini v4"](https://www.amazon.com/ACEIRMC-Internet-ESP8266-Compatible-MicroPython/dp/B0CFY382JX) on Amazon.

### [LOLIN SHT30 Shield](https://www.aliexpress.us/item/2251832575822188.html)

The link above is the official LOLIN Aliexpress store. If this link becomes unavailable, you can use any [SHT30 Breakout Board](https://www.dfrobot.com/product-2012.html) and wire it to a [JST-SH1.0 4P connector](https://www.adafruit.com/product/4209). There is a receptacle labeled I2C on the bottom right of the D1 Mini v4 which accepts this connector.

### [USB-A to USB-C Cable](https://www.amazon.com/tekSonic-10-Pack-Type-C-Cables-Charge/dp/B0CG4GM7PM)

You need one of these for each dry box, to power the sensor inside. You will be chopping the USB-C ends off of these, in order to get the wire through the hole in the box.

The cables I got had fabric weave wrapping - this meant extra complexity because I had to trim the fraying ends and use CA glue to fix them in place 1-2 inches before the end of the USB cable. This gives enough room for stripping the wires later on.

### [USB-C Connector Breakout](https://www.amazon.com/ANMBEST-Connector-Receptacle-Adapter-Support/dp/B091CRLJM2)

In case the Amazon link above stops working, this is what you're looking for:

![usb connector breakout close-up](https://i.imgur.com/Nt6k5Yr.png)

### [M2 Threaded Inserts](https://www.amazon.com/AIEX-Printing-Embedment-Automotive-M2x3x3-5mm/dp/B0B8GN63S2)

These are for the sensor holders.

### [Sterilite 20 qt Boxes](https://www.amazon.com/Sterilite-19344304-Latched-Plastic-Container/dp/B002BA5F3O)

Amazon currently offers 6, 12, 18, and 24 packs. If this listing becomes unavailable, try [Target](https://www.target.com/p/sterilite-20-qt-gasket-box-stackable-storage-bin-with-latching-lid-and-tight-seal-plastic-container-to-organize-basement-clear-base-and-lid-6-pack/-/A-76114127), [Walmart](https://www.walmart.com/ip/Sterilite-20-Quart-Clear-Plastic-Stacking-Storage-Container-with-Gasket-Lid-6-Pack/633637534), or [Home Depot](https://www.homedepot.com/p/Sterilite-20-Qt-Gasket-Box-with-Clear-Base-and-Lid-6-Pack-6-x-19324306/312922371).

### [3/8" Aluminum Tubing](https://www.amazon.com/Tubing-Aluminium-Refrigeration-Equipment-air-conditioning/dp/B0C43X6JR4)

This will be cut to 325 mm (12.75 inch) sections to act as the spool holders.

### [3A Molecular Sieve](https://www.amazon.com/Wisesorb-Molecular-Desiccant-Dehumidifier-Absorbers/dp/B085W6TXJ8)

The 2 pound size would provide for ~150 grams of sieve per box, in a six box setup.

![graph showing molecular sieve taking about 15 minutes to dry to 15 percent RH](./images/graph.png)

## Price Estimate

This is an estimate for a six box, 24-spool setup based on prices I paid in June 2024.

| Parts           | Net Cost |
|-----------------|----------|
| D1 Mini         | $23      |
| SHT30 Shield    | $18      |
| USB Cable       | $12      |
| Molecular Sieve | $18      |
| 20 qt Boxes     | $54      |
| USB Cables      | $6       |
| USB-C Breakouts | $5       |
| Threaded Inserts| $8       |
| Aluminum Tube   | $25      |
| Total           | $169     |

## Tools

 * Drill
 * CA glue
 * Glue Gun
 * Soldering Iron
 * Wire Strippers (12-24 AWG)
 * Belt Sander (or sandpaper and patience)
 * [M2 Threaded Insert Tip for Soldering Iron](https://www.amazon.com/Virtjoule-Heat-Insert-Tips-Sizes/dp/B08B17VQLD)
 * [3/8" Tubing Straightener](https://www.amazon.com/Watris-Veiyi-Straightener-Universal-Straightening/dp/B0BLYYPN91)
 * [Metal Tubing Cutter](https://www.amazon.com/RIDGID-32975-Quarters-Tubing-Cutter/dp/B000LDGNCU)

## 3D Printed Parts

 * Endcaps for aluminum tubing
 * Dessicant tray
 * Sensor mount
