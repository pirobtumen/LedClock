# LedClock
Arduino simple LED clock using 8x8 LED Matrix (MAX7219) and TinyRTC (DS1307).

__WIP__

Part list:
- 1x Arduino
- 3x 8x8 LED Matrix
- 1x TinyRTC module


Read how to use the TinyRTC with Arduino: [Doc](https://github.com/pirobtumen/TinyRTC).


## Prototype Images
![LedClock](https://github.com/pirobtumen/LedClock/blob/master/img/ledclock_prototype.jpg?raw=true)
![LedClock](https://github.com/pirobtumen/LedClock/blob/master/img/ledclock_prototype2.jpeg?raw=true)

## Wiring

![TinyRTC Wiring](https://github.com/pirobtumen/TinyRTC/blob/master/img/TinyRTC_Wiring.jpg?raw=true)

These pins (SDA,SCL) are used to communicate with Arduino through I2C, with the 'Wire' library. [Read more](https://www.arduino.cc/en/Reference/Wire).
