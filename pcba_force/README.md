# MIMOSA Force

Getting Started with Arduino IDE with Adafruit QT Py as Controller
Needless to say, other controllers are also compatible with the MIMOSA Force sensor.
1. Follow [this guide](https://learn.adafruit.com/adafruit-qt-py/using-with-arduino-ide) to add QT Py support into Arduino IDE.
2. Install Mimosa_Force library into Arduino IDE following [this guide](https://www.arduino.cc/en/Guide/Libraries). (Also add external board database from [this guide](https://learn.adafruit.com/add-boards-arduino-v164/setup))
3. Start Arduino IDE. A good starting point is to blink LEDs with a force-triggered button. Check out an example from File -> Examples -> Mimosa_Force.

Default I2C address is 0x48. The device is compatible with both 3.3V and 5V operation and logic levels.

# License
This project is licensed under the Apache v2.0 License - see the [LICENSE.md](LICENSE.md) file for details

# Contributing
Thank you for your interest - please review CONTRIBUTING.md!