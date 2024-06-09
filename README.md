# My Filament Dry Box Adventure

I'm not the first person to do this, but I wanted to document my specific method in case others find it useful.

The Sterilite 20 quart gasket box was chosen because it is popular for this purpose and fits my basic needs - it fits 4 spools of 1kg filament with minimal wasted space and the lid is gasketed.

The LOLIN D1 Mini was chosen as the microcontroller for the humidity sensor part of this project because they are cheap ($3.70 as of this writing), small, and fairly well designed. There is also an SHT30 shield for the D1 mini, which makes the sensor hookup simple. The SHT30 is a fairly accurate sensor (±2% RH) which will suffice for these purposes.

## Bill of Materials

### [LOLIN D1 Mini V4](https://www.aliexpress.us/item/3256806460241404.html)

The link above is the official LOLIN Aliexpress store. If it becomes unavailable, you can find clones of it by searching ["d1 mini v4"](https://www.amazon.com/ACEIRMC-Internet-ESP8266-Compatible-MicroPython/dp/B0CFY382JX) on Amazon.

### [LOLIN SHT30 Shield](https://www.aliexpress.us/item/2251832575822188.html)

The link above is the official LOLIN Aliexpress store. If this link becomes unavailable, you can use any [SHT30 Breakout Board](https://www.dfrobot.com/product-2012.html) and wire it to a [JST-SH1.0 4P connector](https://www.adafruit.com/product/4209). There is a receptacle labeled I2C on the bottom right of the D1 Mini v4 which accepts this connector.

### [USB-A to USB-C Cable](https://www.amazon.com/tekSonic-10-Pack-Type-C-Cables-Charge/dp/B0CG4GM7PM)

You need one of these for each dry box, to power the sensor inside.

### [USB-C Connector Breakout](https://www.amazon.com/ANMBEST-Connector-Receptacle-Adapter-Support/dp/B091CRLJM2)

In case the Amazon link above stops working, this is what you're looking for:

![usb connector breakout close-up](https://i.imgur.com/Nt6k5Yr.png)

### [M2 Threaded Inserts](https://www.amazon.com/AIEX-Printing-Embedment-Automotive-M2x3x3-5mm/dp/B0B8GN63S2)

These are for the sensor holders.

### [Sterilite 20 qt Boxes](https://www.amazon.com/Sterilite-19344304-Latched-Plastic-Container/dp/B002BA5F3O)

Amazon currently offers 6, 12, 18, and 24 packs. If this listing becomes unavailable, try [Target](https://goto.ryukyu/sterilite-target), [Walmart](https://goto.ryukyu/sterilite-walmart), or [Home Depot](https://goto.ryukyu/sterilite-hd).

### [3/8" Aluminum Tubing](https://www.amazon.com/Tubing-Aluminium-Refrigeration-Equipment-air-conditioning/dp/B0C43X6JR4)

This will be cut to 325 mm (12.75 inch) sections to act as the spool holders.

### [3A Molecular Sieve](https://www.amazon.com/Wisesorb-Molecular-Desiccant-Dehumidifier-Absorbers/dp/B085W6TXJ8)

Removes more water faster than silica gel, but takes more heat/time to dry it back out. The 2 pound size would provide for ~150 grams of sieve per box, in a six box setup.

## Price Estimate

This is an estimate for a 24-spool, six box setup based on prices I paid in June 2024.

| Parts           | Net Cost |
|-----------------|----------|
| D1 Mini         | $23      |
| SHT30 Shield    | $18      |
| USB Cable       | $12      |
| Molecular Sieve | $18      |
| 20 qt Boxes     | $54      |
| USB-C Breakouts | $5       |
| Threaded Inserts| $8       |
| Aluminum Tube   | $25      |
| Total           | $163     |

## Tools

 * Drill
 * Glue Gun
 * Soldering Iron
 * Wire Strippers (12-24 AWG)
 * Belt Sander (or sandpaper and patience)
 * [M2 Threaded Insert Tip for Soldering Iron](https://www.amazon.com/Virtjoule-Heat-Insert-Tips-Sizes/dp/B08B17VQLD)
 * [3/8" Tubing Straightener](https://www.amazon.com/Watris-Veiyi-Straightener-Universal-Straightening/dp/B0BLYYPN91)
 * [Metal Tubing Cutter](https://www.amazon.com/RIDGID-32975-Quarters-Tubing-Cutter/dp/B000LDGNCU)

## 3D Printed Parts

 * Endcaps for aluminum tubing (credit to [RCNet](https://www.printables.com/model/139303-sterilite-20-qt-gasket-box-spool-rack))
 * Dessicant tray
 * Sensor mount
