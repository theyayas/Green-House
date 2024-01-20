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

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// VARIABLE THINGER.IO
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

// KONFIGURASI WIFI
const char* ssid = "Bungolang Y";
const char* password = "tanyagading";

// KONSTANTA SOIL MOISTURE SENSOR
const int dry = 720; // Nilai ketika kering
const int wet = 600; // Nilai ketika basah

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
  };
}

void loop() {
  // MEMBACA NILAI SENSOR SOIL MOISTURE
  soilMoistureValue = analogRead(A0);
  soilmoisturepercent = map(soilMoistureValue, dry, wet, 0, 100);
  
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

  lcd.setCursor(2, 0);
  lcd.println(" Condition    ");

  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(t, 1);
  lcd.print("C");
     
  lcd.setCursor(9, 1);
  lcd.print("H:");
  lcd.print(soilmoisturepercent, 1);
  lcd.print("%");

  // ATUR POMPA ON/OFF BERDASARKAN NILAI SENSOR SOIL MOISTURE
  if (soilmoisturepercent > 60){
    digitalWrite(relay, HIGH);
  }
  else if(soilmoisturepercent < 31){
    digitalWrite(relay, LOW);
  }

  thing.handle();
  delay(200);
}
