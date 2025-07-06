/**
 * @file weather_station.ino
 * @brief Simple weather station using BME280, DHT11, Dallas temperature sensor, and LDR.
 */

#include "SparkFunBME280.h"
#include <LightDependentResistor.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include "DHT.h"

/**
 * @brief I2C LCD Display initialization
 */
LiquidCrystal_I2C lcd(0x27, 16, 2);

/**
 * @brief Light sensor setup
 */
#define OTHER_RESISTOR 10000 //ohms
#define USED_PIN A1
#define USED_PHOTOCELL LightDependentResistor::GL5537_2

LightDependentResistor photocell(USED_PIN,
                                 OTHER_RESISTOR,
                                 USED_PHOTOCELL,
                                 10, // Default ADC resolution
                                 10); //Default linear smooth (if used)

/**
 * @brief Dallas temperature sensor setup
 */
OneWire oneWire(A0);
DallasTemperature sensors(&oneWire);
DeviceAddress T5 = {0x28, 0xE4, 0x34, 0x95, 0xF0, 0x1, 0x3C, 0x44};

/**
 * @brief DHT11 temperature and humidity sensor on digital pin 4
 */
DHT dht(4, DHT11);

/**
 * @brief BME280 sensor for atmospheric pressure
 */
BME280 mySensor;

/**
 * @brief Global variables
 */
volatile bool przycisk = false;
int klik = 0;
float jasnosc = 0.0;

/**
 * @brief Interrupt handler for button press
 */
void handleInterrupt() {
  przycisk = true;
}

/**
 * @brief Custom characters for Polish diacritics on LCD
 */
uint8_t user_chars[8][8] = {
    {0x00, 0x00, 0x0E, 0x01, 0x0F, 0x11, 0x0F, 0x02}, // ą
    {0x02, 0x04, 0x0E, 0x10, 0x10, 0x11, 0x0E, 0x00}, // ć
    {0x00, 0x00, 0x0E, 0x11, 0x1F, 0x10, 0x0E, 0x02}, // ę
    {0x0C, 0x04, 0x06, 0x0C, 0x04, 0x04, 0x0E, 0x00}, // ł
    {0x02, 0x04, 0x16, 0x19, 0x11, 0x11, 0x11, 0x00}, // ń
    {0x02, 0x04, 0x0E, 0x11, 0x11, 0x11, 0x0E, 0x00}, // ó
    {0x02, 0x04, 0x0E, 0x10, 0x0E, 0x01, 0x1E, 0x00}, // ś
    {0x02, 0x04, 0x1F, 0x02, 0x04, 0x08, 0x1F, 0x00}  // ź
};

/**
 * @brief Arduino setup function. Initializes sensors, LCD and interrupt.
 */
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  
  sensors.begin();
  
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), handleInterrupt, FALLING);
  
  dht.begin();
  
  mySensor.setI2CAddress(0x76);
  mySensor.begin();

  for (int i = 0; i < 8; i++) {
        lcd.createChar(i, user_chars[i]);
    }
}

/**
 * @brief Arduino loop function. Displays sensor data depending on user mode.
 *
 * User cycles between display modes by pressing a button:
 * - Mode 0: Temperature from DS18B20
 * - Mode 1: Brightness in lux
 * - Mode 2: Humidity from DHT11
 * - Mode 3: Atmospheric pressure from BME280
 */
void loop() {
  if (przycisk) {
      lcd.setCursor(0, 0);
      lcd.write("                ");
      lcd.setCursor(0, 1);
      lcd.write("                ");
    klik++;
    delay(500);
    if (klik > 3) {
      klik = 0;
    }
    przycisk = false;
  }

  sensors.requestTemperatures();
  float intensity_in_lux = photocell.getCurrentLux();

  switch (klik) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.write("Temperatura:");
      lcd.setCursor(0, 1);
      lcd.print(sensors.getTempC(T5));
      lcd.setCursor(6, 1);
      lcd.print((char)223);
      lcd.setCursor(7, 1);
      lcd.print("C       ");
      break;

    case 1:
      lcd.setCursor(0, 0);
      lcd.write("Jasno");
      lcd.write((uint8_t)6);
      lcd.write((uint8_t)1);
      lcd.write(":");
      lcd.setCursor(0, 1);
      lcd.print(intensity_in_lux);
      lcd.print(" lux     ");
      break;

    case 2:
      lcd.setCursor(0, 0);
      lcd.write("Wilgotno");
      lcd.write((uint8_t)6);
      lcd.write((uint8_t)1);
      lcd.write(":");
      lcd.setCursor(0, 1);
      lcd.print(dht.readHumidity());
      lcd.setCursor(6, 1);
      lcd.print("%        ");
      break;

    case 3:
      lcd.setCursor(0, 0);
      lcd.write("Ci");
      lcd.write((uint8_t)6);
      lcd.write("nienie:");
      lcd.setCursor(0, 1);
      lcd.print(mySensor.readFloatPressure());
      lcd.print(" hPa        ");
      break;
  }
}
