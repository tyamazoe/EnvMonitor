//
// Merged M5Stack sample ENVII_SHT30_BMP280.ino into TVOC.ino
//
//SGP30 needs 15 seconds to initialize calibration after power on.
//The screen will display TVOC and CO2
//Added Temperature, Humidity and hPa

#include <M5Stack.h>
#include "Adafruit_SGP30.h"
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "SHT3X.h"

// ENVII
SHT3X sht30;
Adafruit_BMP280 bme;
float tmp = 0.0;
float hum = 0.0;
float pressure = 0.0;

// TVOC
Adafruit_SGP30 sgp;
int i = 15;
long last_millis = 0;

void header(const char *string, uint16_t color)
{
    M5.Lcd.fillScreen(color);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.fillRect(0, 0, 320, 30, TFT_BLACK);
    M5.Lcd.setTextDatum(TC_DATUM);
    M5.Lcd.drawString(string, 160, 3, 4); 
}

void setup() {
  M5.begin(true, false, true, true);
  header("Environment Monitor",TFT_BLACK);
  Serial.begin(115200);
  M5.Lcd.setBrightness(10); // 0-255
  Serial.println("Environment Monitor");
  if (! sgp.begin()){
    Serial.println("Sensor not found :(");
    while (1);
  }
  
  M5.Lcd.drawString("TVOC:", 50, 40, 4);
  M5.Lcd.drawString("eCO2:", 50, 80, 4);
  Serial.print("Found SGP30 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);
  M5.Lcd.drawString("Initialization...", 140, 120, 4);

  // Setup ENV II Unit SHT30 and BMP280 
  Serial.println(F("ENV Unit(SHT30 and BMP280) test..."));
  while (!bme.begin(0x76)){  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    M5.Lcd.println("Could not find a valid BMP280 sensor, check wiring!");
  }
  M5.Lcd.drawString("T  H:", 50, 160, 4);
  M5.Lcd.drawString("AirP:", 50, 200, 4);

  Serial.println("setup done");
}

void loop() {
  while(i > 0) {    
    if(millis()- last_millis > 1000) {
      last_millis = millis();
      i--;
      M5.Lcd.fillRect(198, 120, 40, 20, TFT_BLACK);
      M5.Lcd.drawNumber(i, 20, 120, 4);
    }
  }
  M5.Lcd.fillRect(0, 120, 300, 30, TFT_BLACK);

  // Read TVOC/eCO2
  if (! sgp.IAQmeasure()) {
    Serial.println("Measurement failed");
    return;
  }
  M5.Lcd.fillRect(100, 40, 220, 90, TFT_BLACK);
  M5.Lcd.drawNumber(sgp.TVOC, 120, 40 , 4);
  M5.Lcd.drawString("ppb", 200, 40, 4);
  M5.Lcd.drawNumber(sgp.eCO2, 120, 80, 4);
  M5.Lcd.drawString("ppm", 200, 80, 4);
  Serial.print("TVOC "); Serial.print(sgp.TVOC); Serial.print(" ppb\t");
  Serial.print("eCO2 "); Serial.print(sgp.eCO2); Serial.println(" ppm");
  delay(1000);

  // Read ENV II Unit SHT30 and BMP280 
  pressure = bme.readPressure()/100;
  if(sht30.get()==0){
    tmp = sht30.cTemp;
    hum = sht30.humidity;
  }
  M5.Lcd.drawNumber(tmp, 120, 160, 4);
  M5.Lcd.drawString("C", 160, 160, 4);
  M5.Lcd.drawNumber(hum, 220, 160, 4);
  M5.Lcd.drawString("%", 260, 160, 4);
  M5.Lcd.drawNumber(pressure, 120, 200, 4);
  M5.Lcd.drawString("hPa", 200, 200, 4);
  Serial.printf("Temperatura: %2.2f C  Humedad: %0.2f %%  Pressure: %6.1fhPa\r\n", tmp, hum, pressure);
  delay(1000);
}
