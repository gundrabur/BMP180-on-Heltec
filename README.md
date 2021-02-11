# BMP180-on-Heltec
Use the BMP180 pressure sensor with a Heltec WiFi Kit 32 at the same time as the built-in OLED display

Yes, it is possible to use the standard I2C bus to which the integrated OLED display is connected
of the Heltec WiFi Kit 32 at the same time with an external I2C device (in this case pressure sensor BMP180)

This sketch reads pressure and temperature data from the BMP180 sensor
from the I2C bus and delivers the results on the built-in OLED display
Connect to the same I2C bus

Uses:
- Heltec WiFi Kit 32 (with integrated OLED display)
- BMP180 sensor connected to pin 4 (SDA) and pin 15 (SCL)
- Adafruit BMP085 library
- Arduino Framework

The sketch also includes an I2C scanner that can be used to get the address of the internal OLED display and any external I2C devices connected to both I2C buses.
To switch to I2C scan mode, change #define ScanMode false to true.

Find short video of working device here: https://youtu.be/e1U47JysdzI

Have fun!
