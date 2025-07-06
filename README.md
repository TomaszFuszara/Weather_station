# Weather Station

This is a simple portable weather station built using Arduino-compatible components. It displays environmental readings on a 16x2 I2C LCD screen.

## Features

- **Temperature** (DS18B20 sensor)
- **Humidity** (DHT11 sensor)
- **Brightness** (Light-dependent resistor)
- **Air pressure** (BME280 sensor)

## How it works

- Pressing the hardware button cycles through different display modes.
- LCD screen shows one measurement at a time (temperature, light, humidity, pressure).
- Custom characters are used to support Polish diacritics.

## Hardware

- DS18B20 (OneWire) temperature sensor
- DHT11 humidity and temperature sensor
- BME280 temperature, pressure, and humidity sensor
- Light-dependent resistor (GL5537)
- 16x2 I2C LCD display
- Arduino-compatible board

## Dependencies

Make sure to install the following libraries via Arduino Library Manager:

- `SparkFunBME280`
- `LiquidCrystal_I2C`
- `LightDependentResistor`
- `DallasTemperature`
- `DHT sensor library`