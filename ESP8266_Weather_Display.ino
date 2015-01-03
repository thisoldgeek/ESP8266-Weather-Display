#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#define SSID "<SSID>" // insert your SSID
#define PASS "<PASSWORD" // insert your password
#define LOCATIONID "<LOCATION>" // location id
#define DST_IP "23.222.152.140" //api.wunderground.com

#define ESP_RST 4


SoftwareSerial lcdSerial(2, 3); // RX, TX for debugging

const int buffer=300;

int passNum = 1;

// Array of desired weather conditions 
// These must be in the order received from wunderground!
//
// Also, watch out for repeating field names in returned json structures, 
//  and fields with embedded commas (used as delimiters)


char* conds[]={"\"city\":","\"weather\":","\"temp_f\":","\"relative_humidity\":","\"wind_dir\":","\"wind_mph\":","\"pressure_in\":"};
int num_elements = 7;  // number of conditions you are retrieving, count of elements in conds

char close_brace ='}';
char comma = ',';

void setup()
{  pinMode(ESP_RST,OUTPUT);
  reset();

  Serial.begin(9600);
  Serial.setTimeout(5000);
  lcdSerial.begin(9600); // Print to LCD (or similar display)
  
}
void loop()
{
   reset();
   delay(5000);  //ESP8266 takes a while to restart
 
 // try to connect to wifi
  boolean connected=false;
  for(int i=0;i<5;i++){
    if(connectWiFi()){
      connected = true;
      break;
    }
  }
   
  Serial.println("AT+CIPMUX=0"); // set to single connection mode
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += DST_IP;
  cmd += "\",80";
  Serial.println(cmd);
  //lcdSerial.println(cmd);
  if(Serial.find("Error")) return;
  
  cmd = "GET /api/<YOURKEY>/conditions/q/";
  cmd += LOCATIONID;
  cmd +=".json";
  cmd += " HTTP/1.1\r\nHost: api.wunderground.com\r\n\r\n";
  Serial.print(F("AT+CIPSEND="));
  Serial.println(cmd.length());
  if(Serial.find(">")){
    //lcdSerial.print(">");
  }else{
    Serial.println(F("AT+CIPCLOSE"));
    lcdSerial.println(F("?fConnection timeout"));
    delay(1000);
    return;
  }
  Serial.print(cmd);
  
  unsigned int i = 0; //timeout counter
  char json[buffer]="{"; // array for Json parsing
  int n = 1;          // character counter for json  
  
  for (int j=0;j<num_elements;j++){
    while (!Serial.find(conds[j])){} // find the part we are interested in.
  
    String Str1 = conds[j];
  
    for (int l=0; l<(Str1.length());l++)
        {json[n] = Str1[l];
         n++;}   
    while (i<60000) {
      if(Serial.available()) {
        char c = Serial.read();
         
          if(c==',') break;
          json[n]=c;
          n++;
          i=0;
        }
        i++;
      }
     if (j==num_elements-1)
        {json[n]=close_brace;}
     else   
        {json[n]=comma;}
     n++;   
  }

parseJSON(json);

// Done with processing for now - close connection
Serial.println(F("AT+CIPCLOSE"));

  delay(5000);

 // Only check weather every 15 minutes
 // So you don't go over quota on wunderground (for free api license)
  delay(900000);
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

 
 const char* city = root["city"];
 const char* weather = root["weather"];
 double temp_f = root["temp_f"];
 const char* humidity = root["relative_humidity"];
 const char* wind_dir = root["wind_dir"];
 double wind_mph = root["wind_mph"];
 const char* pressure_in = root["pressure_in"];
 
 
 // location of conditions
 lcdSerial.print("?f    ");
 lcdSerial.print(city);
 
 // Conditions: Sunny, Cloudy, Fog, Rain, etc. 
 lcdSerial.print(F("?n"));
 lcdSerial.print(weather);
  
 // Temp
 lcdSerial.print(F("?n"));
 lcdSerial.print((int)temp_f);
 lcdSerial.print(F("F|"));

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


void reset()
{
  digitalWrite(ESP_RST,LOW);
  delay(100);
  digitalWrite(ESP_RST,HIGH);
 
}


boolean connectWiFi()
{
  Serial.println(F("AT+CWMODE=1"));
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  delay(2000);
  if(Serial.find("OK")){
    lcdSerial.println(F("?fConnected to WiFi..."));
    delay(500);     // Allow display to print
    return true;
  }else{
    lcdSerial.println(F("?fCan't Connect..."));
    delay(500);
    return false;
  }
}
