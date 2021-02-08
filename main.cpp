/*
Version 1.0
2021/02/07
Christian Möller
This sketch reads pressure and temperatue data from BMP180 sensor
from I2C bus and dispalys the results on the built-in OLED display
connnect to the same I2C bus
Uses:
- Heltec WiFi Kit 32 (with built in OLED display)
- BMP180 sensor
- Adafruit BMP085 Library

Changes:
2021/02/08
- The air pressure is now displayed with 2 decimal places
- Text output optimized for font size
*/


#include <heltec.h>          // Heltec Library
#include <Adafruit_BMP085.h> // BMP085 Library from Adafruit, runs fine on the WiFi Kit 32

// ##### scan mode! Set this value to true if you only want to discover devices on both I2C buses
// Results are only passed on to the serial monitor!
#define ScanMode false // default is false

// Store an instance of the BMP180 sensor
Adafruit_BMP085 bmp;

// struct definition for storing results
struct bmpResults
{
  float temperature;
  int pressure;
  float altitude;
  int seaLevelPressure;
};

// Pause between the output of the different values
#define Delay 2000 // two seconds

// Calibrate the altitude by changing this default pressure value for your location
#define MyAltitude 99500 // 99500 Pascal is the standard value for Munich / Germany (approx. 525 m altitude)

// ######## The following is only used in scan mode!
// first I2C bus, the OLED display is also connected to this bus
#define SDA1 4
#define SCL1 15
// second I2C bus, will NOT be used in this example!
#define SDA2 21
#define SCL2 22

// Initialize and instantiate both I2C buses
TwoWire I2Cone = TwoWire(0);
TwoWire I2Ctwo = TwoWire(1);

void scan1()
{
  Serial.println("Scanning I2C Addresses Channel 1");
  uint8_t cnt = 0;
  for (uint8_t i = 0; i < 128; i++)
  {
    I2Cone.beginTransmission(i);
    uint8_t ec = I2Cone.endTransmission(true);
    if (ec == 0)
    {
      if (i < 16)
        Serial.print('0');
      Serial.print(i, HEX);
      cnt++;
    }
    else
      Serial.print("..");
    Serial.print(' ');
    if ((i & 0x0f) == 0x0f)
      Serial.println();
  }
  Serial.print("Scan Completed, ");
  Serial.print(cnt);
  Serial.println(" I2C Devices found.");
}
void scan2()
{
  Serial.println("Scanning I2C Addresses Channel 2");
  uint8_t cnt = 0;
  for (uint8_t i = 0; i < 128; i++)
  {
    I2Ctwo.beginTransmission(i);
    uint8_t ec = I2Ctwo.endTransmission(true);
    if (ec == 0)
    {
      if (i < 16)
        Serial.print('0');
      Serial.print(i, HEX);
      cnt++;
    }
    else
      Serial.print("..");
    Serial.print(' ');
    if ((i & 0x0f) == 0x0f)
      Serial.println();
  }
  Serial.print("Scan Completed, ");
  Serial.print(cnt);
  Serial.println(" I2C Devices found.");
}

void drawString(String output, int fontSize)
{
  // outputs a string to the built-in OLED display
  // on random locations to prevent burn in

  // calculating the max value of x position on the display
  // depending on the length of the string and the font size
  unsigned int maxX = 128 - (output.length() * (fontSize / 2));
  unsigned int maxY = 42; // one line of text, defalts to large font
  // text appears in a random position on the display so that the OLED does not burn in
  int randx = 0; // default 0
  int randy = 0; // default 0

  Heltec.display->clear();                           // clear the display
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT); // text will be aligned to the left
  // there are only 3 font sizes to choose from!
  switch (fontSize)
  {
  case 10:
    Heltec.display->setFont(ArialMT_Plain_10); // Text size
    maxY = 54; // maximum y position depends on font size
    break;
  case 16:
    Heltec.display->setFont(ArialMT_Plain_16); // Text size
    maxY = 49; // maximum y position depends on font size
    break;
  case 24:
    Heltec.display->setFont(ArialMT_Plain_24); // Text size
    maxY = 42; // maximum y position depends on font size
    break;
  default:
    Heltec.display->setFont(ArialMT_Plain_16); // Text size
    maxY = 49; // maximum y position depends on font size
    break;
  }

  // text appears in a random position on the display so that the OLED does not burn in
  randx = random(0, maxX); // random X postion
  randy = random(0, maxY); // random Y position

  Heltec.display->drawString(randx, randy, output); // draw the string
  Heltec.display->display();                        // transfer it to display
}

void setup()
{
  Serial.begin(115200); // initialize serial monitor

  // initialize internal display first!
  // this is important because it also makes the
  // I2C bus activated, which we need for the BMP sensor
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
  Heltec.display->resetOrientation();
  Heltec.display->flipScreenVertically(); // flip display if needed
  //Heltec.display->mirrorScreen();    // mirror display if needed e.g. on a HUD
  Heltec.display->setFont(ArialMT_Plain_10); // use this font

  if (!ScanMode)
  {
    // sensor reading mode
    if (!bmp.begin()) // checking if sensor is available
    {
      Serial.println("Could not find a valid BMP085 sensor, check wiring!");
      while (1)
      {
        // we are stuck here, no sensor found!
      }
    }
  }
  else
  {
    // scan mode
    I2Cone.begin(SDA1, SCL1, 400000); // SDA pin 4, SCL pin 15 - built-in OLED
    I2Ctwo.begin(SDA2, SCL2, 400000); // SDA pin 21, SCL pin 22, second I2C bus
  }
}

void loop()
{
  String output; // storing the output text

  if (!ScanMode)
  {
    // We are in sensor reading mode!
    // Retrieve results from the sensor and save them in a struct
    bmpResults allSensorResults = {
        .temperature = bmp.readTemperature(),
        .pressure = bmp.readPressure(),
        .altitude = bmp.readAltitude(MyAltitude),
        .seaLevelPressure = bmp.readSealevelPressure(),
    };

    // output all results to serial monitor first
    Serial.print("Temperature = ");
    Serial.print(allSensorResults.temperature);
    Serial.println(" *C");

    Serial.print("Pressure = ");
    Serial.print(allSensorResults.pressure);
    Serial.println(" Pa");

    // Calculate altitude assuming 'standard' barometric
    // pressure of 101325 Pascal
    Serial.print("Altitude = ");
    Serial.print(allSensorResults.altitude);
    Serial.println(" meters");

    Serial.print("Pressure at sealevel (calculated) = ");
    Serial.print(allSensorResults.seaLevelPressure);
    Serial.println(" Pa");

    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(MyAltitude));
    Serial.println(" meters");

    Serial.println();

    // output all data to OLED display
    output = String(float(allSensorResults.pressure) / 100) + " hPa";
    drawString(output, 16); // medium font size 16, it can be a real long string like "1199.99 hPa"
    delay(Delay);

    output = String(allSensorResults.temperature) + " *C"; // large font 24
    drawString(output, 24);
    delay(Delay);

    output = String(allSensorResults.altitude) + " m"; // large font 24
    drawString(output, 24);
    delay(Delay);
  }
  else
  {
    // we are in scan mode!
    scan1(); // first I2C bus
    Serial.println();
    delay(100);
    scan2(); // second I2C bus
    Serial.println();
    delay(5000); // wait 5 seconds
  }
}
