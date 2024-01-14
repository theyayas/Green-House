#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h> 

String URL = "http://api.thingspeak.com/update?api_key=530UMT8FVADWS7MU&field1="; //ambil link dari thingspeak, jadiin http, dan hapus nilai dari field 1 untuk newURL nanti
String ssid = "Perpustakaan Bergerak Free Wifi";
String password = "vomi0406";

void setup() {
  Serial.begin(9600);

  WiFi.disconnect();
  delay(2000);
  Serial.print("Start connecting");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED))) {
    delay(200);
    Serial.print("..");
  }
  Serial.println("Connected");

}

void loop() {
  if (Serial.available()>0){
    String data = Serial.readStringUntil('\n'); //membaca baris
    Serial.println(data);
    int comma = data.indexOf(',');  // akan digunakan sebagai sebagai delimiter
    if (comma != -1){
      float t = data.substring(0, comma).toFloat(); // mengambil data 0 sampai comma
      float h = data.substring(comma+1).toFloat();  // mengambil data setelah comma
      Serial.print(t);
      Serial.println(h);

      sendData(t,h);
    }
  }
  delay(500);
}

void sendData(float t, float h) {
  WiFiClient client;
  HTTPClient http;
  String newURL = URL + String(t) + "&field2=" + String(h);
  
  http.begin(client, newURL);
  int responsecode = http.GET();
  String data = http.getString();
  Serial.print(responsecode);
  Serial.println(data);
  http.end();
}


