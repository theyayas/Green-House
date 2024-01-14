#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>

#define dhtPin 2
#define dhtType DHT22

DHT dht(dhtPin, dhtType);
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display,Pls check your lcd.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Temperature and Humidity Sensor Test");
  dht.begin();
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight
}

void loop() {
  // put your main code here, to run repeatedly:
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (isnan(t) || isnan(h)){
    Serial.println("No data Available");
  }
  Serial.print(t);
  Serial.print(",");
  Serial.println(h);

  lcd.setCursor(2, 0);
  lcd.println(" Condition    ");
    
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(t, 1);
  lcd.print("C");

  //lcd.setCursor(6, 1);
  //lcd.println("2021 ");
     
  lcd.setCursor(9, 1);
  lcd.print("H:");
  lcd.print(h, 1);
  lcd.print("%");
    
  delay(500); //Delay 1 sec.
}
