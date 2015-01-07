# **ESP8266 Weather Display**
## *Description:*
This is an Arduino project that uses the cheap ESP8266 WiFi module. 
For more details see the blog posting: http://thisoldgeek.blogspot.com/2015/01/esp8266-weather-display.html


The sketch connects to wunderground.com and returns current weather conditions
for your location every 15 minutes. The returned data is formatted as JSON. The
JSON is parsed for display on a serial LCD.

## *Configuration:*
* You will need an API key (free) from http://www.wunderground.com/weather/api/d/login.html
* You must install ArduinoJson in your sketchbook/libraries folder (https://github.com/bblanchon/ArduinoJson)
* You must be using Arduino 1.0.6 or later for ArduinoJson library to install properly - it uses nested include statements
* Change the following:
  * SSID - name of your network
  * PASS - password for your network
  * LOCATIONID - desired geographic location for weather conditions (eg., US ZipCode, State/City, Int'l)
  * YOURKEY - your API key for wunderground

**LOCATIONID** can be of the following forms:
* #define LOCATIONID "Germany/Munich"  (International)
* #define LOCATIONID "94523"           (US ZipCode)
* #define LOCATIONID "CA/Pleasant_Hill" (US City)

## *Process:*
* Hard Reset
* Connect to WiFi
* Set to single connection
* Set up TCP connection
* Send data (GET) to wunderground
* Use Serial to read the returned json (JavaScript Object Notation) pairs
* Parse the key-value pairs
* Display 
* Close the connection
* Wait 15 minutes (to keep within limits of free API license)

# *Pro Trinket Notes Updated 01-07-2015*
The Pro Trinket 3V runs at 12Mhz. SoftwareSerial (required for this project) through Arduino 1.0.6
only has a version of SoftwareSerial that supports 8, 16 and 20MHz. You will need to update the 
SoftwareSerial.cpp file, available here.

Instructions for updating SoftwareSerial.cpp on a Mac:
* Close Arduino if running
* Open a New Finder Window
* Open Applications, highlight Arduino (or whatever you name your Arduino app)
* Show Package Contents
* Open Resources\Java\libraries\SoftwareSerial
* Copy SoftwareSerial.cpp (downloaded from github) to this folder, replacing the older version of SoftwareSerial.cpp
* Restart Arduino





























