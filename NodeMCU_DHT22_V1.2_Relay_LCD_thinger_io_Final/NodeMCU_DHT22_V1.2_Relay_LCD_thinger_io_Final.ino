#include <ThingerESP8266.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>

// KONFIGURASI THINGER.IO
#define USERNAME "theyayas"
#define DEVICE_ID "NodeMCU_DHT22"
#define DEVICE_CREDENTIAL "Tmi9VdBiGHtC8scg"

// KONFIGURASI DHT22
#define dhtPin 4
#define dhtType DHT22
DHT dht(dhtPin, dhtType);

// PIN UNTUK RELAY
#define relay 5
String relay2;

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// VARIABLE THINGER.IO
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

// KONFIGURASI WIFI
const char* ssid = "KANTORDESA_BUMDES";
const char* password = "11112222";

// KONSTANTA SOIL MOISTURE SENSOR
const int dry = 675; // Nilai ketika kering 689
const int wet = 609; // Nilai ketika basah 
int soilMoistureValue = 0;
int soilmoisturepercent = 0;

// VARIABLE BACA DATA SENSOR
float t, h;

void setup() {
  Serial.begin(9600);
  Wire.begin(2,0);

  dht.begin();
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 

  // SETTINGAN PIN
  pinMode(relay, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT); 

  digitalWrite(relay, HIGH); // untuk set pin relay agar OFF. Default LOW, maka relay ON

  // KONEKSI KE WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("..");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100); // sebagai indikator connecting ke wifi; setelah connect, LED akan mati/hidup
  }

  // HUBUNGKAN KE THINGER.IO
  thing.add_wifi(ssid, password);

  // MENERIMA DATA DARI THINGER.IO
  thing["led"] << digitalPin(relay);

  // MENGIRIM DATA KE THINGER.IO
  thing["DHT22"] >> [](pson & out) {
    out["Temperature"]  = t;
    out["Humidity"]     = soilmoisturepercent;
    out["Pompa"]        = relay2;
    out["soilmoisture"] = soilMoistureValue; // BACA NILAI DI THINGER.IO
  };
}

void loop() {
  // MEMBACA NILAI SENSOR SOIL MOISTURE
  soilMoistureValue = analogRead(A0);
  soilmoisturepercent = map(soilMoistureValue, dry, wet, 0, 100);

  Serial.println(soilMoistureValue);
  
  // MEMBACA DATA DARI ARDUINO UNO
  /*if (Serial.available()>0){
    String data = Serial.readStringUntil('\n'); //membaca baris
    Serial.println(data);

    int comma = data.indexOf(',');  // akan digunakan sebagai sebagai delimiter
    if (comma != -1){
      t = data.substring(0, comma).toFloat(); // mengambil data 0 sampai comma
      h = data.substring(comma+1).toFloat();  // mengambil data setelah comma
      Serial.print(t);
      Serial.println(h);
    }
  }*/

  // BACA SENSOR DHT22
  t = dht.readTemperature();
  h = dht.readHumidity();

  //lcd.setCursor(2, 0);
  //lcd.println(" Condition    ");

  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(t, 1);
  lcd.print("C");
     
  lcd.setCursor(9, 1);
  lcd.print("H:");
  lcd.print(soilmoisturepercent, 1);
  lcd.print("%      ");

  // ATUR POMPA ON/OFF BERDASARKAN NILAI SENSOR SOIL MOISTURE
  if (soilmoisturepercent > 83){
    digitalWrite(relay, HIGH);
    lcd.setCursor(2, 0);
    lcd.println(" POMPA : OFF    ");
    //lcd.println(soilMoistureValue);
    relay2 = "POMPA OFF";
  }
  else if(soilmoisturepercent < 37){
    digitalWrite(relay, LOW);
    lcd.setCursor(2, 0);
    lcd.println(" POMPA : ON    ");
    //lcd.println(soilMoistureValue);
    relay2 = "POMPA ON";
  }

  thing.handle();
  delay(500);
}
