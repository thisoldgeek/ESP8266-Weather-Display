/*
 * Changes Based on:
 * https://github.com/mike-rankin/ESP8266_OLED_Display
 */
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#define SSID "Your SSID" // insert your SSID
#define PASS "Your Password" // insert your password
#define LOCATIONID "94523" // location id
//#define HOST "23.222.152.140" //api.wunderground.com
#define HOST "api.wunderground.com"
#define YOUR_KEY "INSERT your wunderground api key HERE" // your (free) license_key 

const int buffer_size = 300;                         // length of json buffer

SoftwareSerial lcdSerial(4, 5); // RX, TX for debugging

const int buffer=300;

int passNum = 1;

// Array of desired weather conditions 
// These must be in the order received from wunderground!
//
// Also, watch out for repeating field names in returned json structures, 
//  and fields with embedded commas (used as delimiters)


char* conds[]={
   "\"city\":",
   "\"weather\":",
   "\"temp_f\":",
   "\"relative_humidity\":",
   "\"wind_dir\":",
   "\"wind_mph\":",
   "\"pressure_in\":"
};

int num_elements = 7;  // number of conditions you are retrieving, count of elements in conds

unsigned long WMillis = 0;                           // temporary millis() register

void setup()
{  Serial.begin(115200);                              // baudrate of monitor
   WiFi.begin(SSID,PASS);                             // your WiFi Network's SSID & Password
    while (WiFi.status() != WL_CONNECTED) {          // DO until connected
    delay(500);                                      // 
    Serial.print(".");                               // print a few dots
  }
  Serial.println("");                            
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

   
  lcdSerial.begin(9600); // Print to LCD (or similar display)
  wunderground();
}


void loop()
{
     // Only check weather every 5-15 minutes, so you don't go over quota on wunderground (for free api license)
     if (millis()-WMillis >= 600000) {                // 300 seconds interval
      wunderground();                                // get new data
      WMillis=millis();                              // 
     }
     // enough time here to do other stuff, like a bar that is showing how long it takes to update.
}

void wunderground() 
{
  Serial.print("connecting to ");
  Serial.println(HOST);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  
  const int httpPort = 80;
  
  if (!client.connect(HOST, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  String cmd = "GET /api/";  cmd += YOUR_KEY;                   // build request_string cmd
  cmd += "/conditions/q/";  cmd += LOCATIONID;  cmd +=".json";  //
  cmd += " HTTP/1.1\r\nHost: api.wunderground.com\r\n\r\n";     // 
  delay(500);
  client.print(cmd);                                            // connect to api.wunderground.com with request_string
  delay(500);
  unsigned int i = 0;                                           // timeout counter
  char json[buffer_size]="{";                                   // first character for json-string is begin-bracket 
  int n = 1;                                                    // character counter for json  
  
  for (int j=0;j<num_elements;j++){                             // do the loop for every element/condition
    boolean quote = false; int nn = false;                      // if quote=fals means no quotes so comma means break
    while (!client.find(conds[j])){}                            // find the part we are interested in.
  
    String Str1 = conds[j];                                     // Str1 gets the name of element/condition
  
    for (int l=0; l<(Str1.length());l++)                        // for as many character one by one
        {json[n] = Str1[l];                                     // fill the json string with the name
         n++;}                                                  // character count +1
    while (i<5000) {                                            // timer/counter
      if(client.available()) {                                  // if character found in receive-buffer
        char c = client.read();                                 // read that character
          // Serial.print(c);                                   // 
// ************************ construction of json string converting comma's inside quotes to dots ********************        
               if ((c=='"') && (quote==false))                  // there is a " and quote=false, so start of new element
                  {quote = true;nn=n;}                          // make quote=true and notice place in string
               if ((c==',')&&(quote==true)) {c='.';}            // if there is a comma inside quotes, comma becomes a dot.
               if ((c=='"') && (quote=true)&&(nn!=n))           // if there is a " and quote=true and on different position
                  {quote = false;}                              // quote=false meaning end of element between ""
               if((c==',')&&(quote==false)) break;              // if comma delimiter outside "" then end of this element
 // ****************************** end of construction ******************************************************
          json[n]=c;                                            // fill json string with this character
          n++;                                                  // character count + 1
          i=0;                                                  // timer/counter + 1
        }
        i++;                                                    // add 1 to timer/counter
      }                    // end while i<5000
     if (j==num_elements-1)                                     // if last element
        {json[n]='}';}                                          // add end bracket of json string
     else                                                       // else
        {json[n]=',';}                                          // add comma as element delimiter
     n++;                                                       // next place in json string
  }
  //Serial.println(json);                                       // debugging json string 
  parseJSON(json);                                              // extract the conditions
  WMillis=millis();
}


void parseJSON(char json[300])
{
  StaticJsonBuffer<buffer> jsonBuffer;
 JsonObject& root = jsonBuffer.parseObject(json);
 
 if (!root.success())
{
  lcdSerial.print("?fparseObject() failed");
  //return;
}

 
 const char* city         = root["city"];
 const char* weather      = root["weather"];
 double temp_f            = root["temp_f"];
 const char* humidity     = root["relative_humidity"];
 const char* wind_dir     = root["wind_dir"];
 double wind_mph          = root["wind_mph"];
 const char* pressure_in  = root["pressure_in"];
 
 
 // location of conditions
 lcdSerial.print("?f    ");
 lcdSerial.print(city);
 Serial.println(city);
 // Conditions: Sunny, Cloudy, Fog, Rain, etc. 
 lcdSerial.print(F("?n"));
 lcdSerial.print(weather);
 Serial.println(weather);
 // Temp
 lcdSerial.print(F("?n"));
 lcdSerial.print((int)temp_f);
 lcdSerial.print(F("F|"));
Serial.println((int)temp_f);
// Humidity 
 lcdSerial.print(humidity);
 lcdSerial.print(F("|"));
 
 // Wind Direction
 lcdSerial.print(wind_dir);
 lcdSerial.print(F(" "));
 
  // Wind Speed
 lcdSerial.print((int)wind_mph);
 lcdSerial.print(F(" "));
 lcdSerial.print(F("MPH"));
 
 
 // Barometric Pressure
 lcdSerial.print(F("?nBarometer: "));
 lcdSerial.print(pressure_in);
  
}



