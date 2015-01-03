# **ESP8266 Weather Display**
## *Description:*
This is an Arduino project that uses the cheap ESP8266 WiFi module. 

The sketch connects to wunderground.com and returns current weather conditions
for your location every 15 minutes. The returned data is formatted as JSON. The
JSON is parsed for display on a serial LCD.

## *Configuration:*
* You will need an API key (free) from http://www.wunderground.com/weather/api/d/login.html
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
*Hard Reset
*Connect to WiFi
*Set to single connection
*Set up TCP connection
*Send data (GET) to wunderground
*Use Serial to read the returned json (JavaScript Object Notation) pairs
*Parse the key-value pairs
*Display 
*Wait 15 minutes (to keep within limits of free API license)
























