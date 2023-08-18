#include<Wire.h> 
#include<LiquidCrystal_I2C.h>
#include<DallasTemperature.h> 
#include "DHT.h"
LiquidCrystal_I2C lcd(0x27, 16, 2);

int klik = 0;
float jasnosc = 0.0;
OneWire oneWire(A0); //Podłączenie do A0
DallasTemperature sensors(&oneWire); //Przekazania informacji do biblioteki
DeviceAddress T5 = {0x28, 0xE4, 0x34, 0x95, 0xF0, 0x1, 0x3C, 0x44}; //Zapisanie adresu
//{0x28, 0xFF, 0x64, 0x0E, 0x75, 0x76, 0xC4, 0x3E};
DHT dht(2, DHT11);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  sensors.begin();
  pinMode(4, INPUT_PULLUP); 
  dht.begin();
}

void loop() {
  sensors.requestTemperatures();
  if(digitalRead(4) == LOW){
  delay(100);
  lcd.setCursor(0,0);
  lcd.write("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  klik++;
  if(klik > 2){
    klik = 0;
  }
}

switch (klik){
  case 0:
  lcd.setCursor(0,0);
  lcd.write("Temperatura:");
  lcd.setCursor(0,1);
  lcd.print(sensors.getTempC(T5));
  lcd.setCursor(5,1);
  lcd.print((char)223);
  lcd.setCursor(6,1);
  lcd.print("C");
  delay(100);
  break;
  
  case 1:
  lcd.setCursor(0,0);
  lcd.write("Jasnosc:");
  lcd.setCursor(0,1);
  jasnosc = 100.0-((analogRead(A1)/1024.0)*100.0);
  lcd.print(jasnosc);
  lcd.setCursor(5,1);
  lcd.print("%");
  delay(100);
  break; 

  case 2:
  lcd.setCursor(0,0);
  lcd.write("Wilgotnosc:");
  lcd.setCursor(0,1);
  lcd.print(dht.readHumidity());
  lcd.setCursor(5,1);
  lcd.print("%");
  delay(100);
  break;
}
}
