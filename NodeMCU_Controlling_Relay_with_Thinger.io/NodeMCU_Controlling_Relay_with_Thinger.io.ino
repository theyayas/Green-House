#include <ThingerESP8266.h>
#include <ESP8266WiFi.h>

// KONFIGURASI THINGER.IO
#define USERNAME "fileyas"
#define DEVICE_ID "Switch"
#define DEVICE_CREDENTIAL "UdrjW1To&LcrA5lQ"

// VARIABLE THINGER.IO
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

// KONFIGURASI WIFI
const char* ssid = "KOPI DARI HATI";
const char* password = "kopidarihatipetung3";

#define relay 5

void setup() {
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(relay, OUTPUT);

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

  //thing["led"] << [](pson& in){
  //  digitalPin(relay);
  //};
}

// the loop function runs over and over again forever
void loop() {
  thing.handle();                     // wait for a second
}
