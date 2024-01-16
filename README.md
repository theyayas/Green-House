# What to know
This project is focused on Green House Project using DHT22 Humidity Sensor, thinger.io as the IoT server, and Water Pump as the actuator. The server is expected to do "monitoring and controlling the green house condition"

# Installation and Configurations
1. Download Arduino IDE from https://www.arduino.cc/en/software and install it
2. Install CH340 Driver from https://www.arduined.eu/ch340-windows-10-driver-download/
3. To configure the ESP8266 NodeMCU board;
   - Open Arduino IDE, go to File -> Preferences, paste "http://arduino.esp8266.com/stable/package_esp8266com_index.json" into Additional Board Manager URLs
   - Go to Tools -> Board -> Boards Manager, type "ESP8266" in the search bar, then install it
4. To install the libraries needed for IoT Server and/or sensors;
   - Go to Sketch -> Include Library -> Manage Libraries, then install "thinger.io", "LiquidCrystal I2C", "DHT", and all needed libaries
5. You're good to go
