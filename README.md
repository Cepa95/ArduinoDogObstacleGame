# ArduinoDogObstacleGame

## Overview
The Arduino Dog Obstacle Game is a fun little project that showcases the endless possibilities with Arduino boards. You get to guide a virtual pup through obstacles using a button connected to the Arduino. It's all thanks to the inspiration and guidance from Arduino projects that I was able to bring this game to life.
## How to Configure&Play
1. Connect the components as follows:
   - Connect one side of the button to ground (GND) on the Arduino.
   - Connect the other side of the button to digital pin 2 on the Arduino.
   - Connect the LCD module:
     - Connect GND to GND on the Arduino.
     - Connect VCC to 5V on the Arduino.
     - Connect SDA to A4 on the Arduino.
     - Connect SCL to A5 on the Arduino.
     - ![schema](https://github.com/Cepa95/ArduinoDogObstacleGame/assets/124800316/737bf51b-6aa9-4a93-aabe-a1fbcc83303a)


2. Once the components are connected, upload the provided Arduino code to your Arduino board.

3. Power on the Arduino and the LCD module.

4. Press the button to guide the virtual pup through the obstacles displayed on the LCD screen.

## JSON Configuration
If you're using the Wokwi simulator, you can use the provided JSON configuration to set up the components in the simulator environment.

```json
{
  "version": 1,
  "author": "Cepa95",
  "editor": "wokwi",
  "parts": [
    { "type": "wokwi-breadboard-mini", "id": "bb1", "top": -30.2, "left": 199.2, "attrs": {} },
    { "type": "wokwi-arduino-uno", "id": "uno", "top": 106.2, "left": 124.2, "attrs": {} },
    {
      "type": "wokwi-pushbutton",
      "id": "btn1",
      "top": -10.2,
      "left": 300.6,
      "rotate": 90,
      "attrs": { "color": "green" }
    },
    {
      "type": "wokwi-lcd1602",
      "id": "lcd1",
      "top": 140.8,
      "left": 418.4,
      "attrs": { "pins": "i2c" }
    }
  ],
  "connections": [
    [ "bb1:15b.g", "uno:2", "green", [ "v0" ] ],
    [ "bb1:13b.g", "uno:GND.1", "black", [ "v0" ] ],
    [ "lcd1:GND", "uno:GND.3", "black", [ "h0" ] ],
    [ "lcd1:VCC", "uno:5V", "red", [ "h0" ] ],
    [ "lcd1:SDA", "uno:A4", "gold", [ "h0" ] ],
    [ "lcd1:SCL", "uno:A5", "orange", [ "h0" ] ],
    [ "btn1:1.l", "bb1:15t.a", "", [ "$bb" ] ],
    [ "btn1:2.l", "bb1:13t.a", "", [ "$bb" ] ],
    [ "btn1:1.r", "bb1:15b.f", "", [ "$bb" ] ],
    [ "btn1:2.r", "bb1:13b.f", "", [ "$bb" ] ]
  ],
  "dependencies": {}
}
```

## Demo video


https://github.com/Cepa95/ArduinoDogObstacleGame/assets/124800316/b4e09c7c-c315-439b-a4fc-1d2456df8c2a

