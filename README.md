# BMP180-on-Heltec
Use pressure sensor BMP180 on a Heltec WiFi Kit 32 simultaneous with built in OLED display 

Yes, it is pissible to use the standard I2C bus to which the built-in OLED display
of the Heltec WiFi Kit 32 simultaneously with an external I2C decive (am prssure sensor BMP180 in this case)

This sketch reads pressure and temperatue data from BMP180 sensor
from I2C bus and dispalys the results on the built-in OLED display
connnect to the same I2C bus

Uses:
- Heltec WiFi Kit 32 (with built in OLED display)
- BMP180 sensor
- Adafruit BMP085 Library
- Arduino Framework

The sketch also features an I2C scanner to retreive the adress of then internal OLED display and any external I2C device connected to both I2C buses.
To switch to I2C scan mode change #define ScanMode false to true.

