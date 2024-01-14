#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN 0   //nodemcu pin D3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);


String apiKey = "5YMJ412GE9LLTRK8";     //  Enter your Write API key from ThingSpeak
const char* resource = "/update?api_key=";

const char *ssid =  "Galaxy A50sD183";     // replace with your wifi ssid and wpa2 key
const char *pass =  "whta9478";
const char* server = "api.thingspeak.com";

WiFiClient client;

void setup() 
{
  
       Serial.begin(115200);
       delay(10);

       Serial.println("Connecting to ");
       Serial.println(ssid);

// Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");


      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");

      dht.begin();
}



void loop() 
{

  int h = dht.readHumidity();
  int t = dht.readTemperature(true); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  

                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr += "\r\n\r\n";

                             client.print(String("GET ") + resource + apiKey + "&field1=" + t + "&field2=" + h + " HTTP/1.1\r\n" + "Host: " + server + "\r\n" + "Connection: close\r\n\r\n");
                             //client.print(String("GET ") + resource + apiKey + "&field1=" + t + " HTTP/1.1\r\n" + "Host: " + server + "\r\n" + "Connection: close\r\n\r\n");                             
                                        
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
                             
                             Serial.print("Temperature: ");
                             Serial.print(t);
                             Serial.print(" humidity: ");
                             Serial.print(h);
                             Serial.println(". Send to Thingspeak.");
                        }
                        
          client.stop();

          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(500);
}