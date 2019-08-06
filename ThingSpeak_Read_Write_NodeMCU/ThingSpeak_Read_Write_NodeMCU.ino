#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char ssid[] = "Android";   // your network SSID (name) 
const char pass[] = "shri@1998";   // your network password

WiFiClient  client;

unsigned long TSChannelID = 832256;//SECRET_CH_ID;
const char * TSReadAPIKey = "TSK7GHPHIYD5B1UH";//SECRET_READ_APIKEY;
String TSWriteAPIKey = "L1KS52PVM3GIYCCF";//SECRET_WRITE_APIKEY;
unsigned int TSFieldNumber = 1; 

void setup() {
  Serial.begin(115200);  // Initialize serial

  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  Write_ThingSpeak_Data(String(12)); // pass the sensor data to function 
  Read_ThingSpeak_Data();
}

void ConnectToWiFi(void){
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print("Connecting...");
      delay(5000);     
    } 
    Serial.print("\nConnected\t" + String(ssid) + "SSID\n");
  }
}

void Write_ThingSpeak_Data(String sensor){
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) { //Check WiFi connection status
    Serial.println("WiFi Disconected ?!!");
    ConnectToWiFi();
  }
  HTTPClient http;  //Declare an object of class HTTPClient
  String upLink = "http://api.thingspeak.com/update?api_key="+TSWriteAPIKey+"="; // ThingSpeak Write API key paste here
  upLink += String(sensor);  
  Serial.println(upLink);
  http.begin(upLink);  //Specify request destination
  int httpCode = http.GET();  //Send the GET request
  http.end();   //Close connection
  delay(15000);    //Send a request every 15 seconds
}

void Read_ThingSpeak_Data(void){
  int statusCode = 0;
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) { //Check WiFi connection status
    Serial.println("WiFi Disconected ?!!");
    ConnectToWiFi();
  }

  float readData = ThingSpeak.readFloatField(TSChannelID, TSFieldNumber, TSReadAPIKey);
  Serial.println(">>>>>> " + String(readData));
  
  statusCode = ThingSpeak.getLastReadStatus();
  if(statusCode == 200){
    Serial.println("Data ----> " + String(readData));
  }
  else{
    Serial.println("Problem reading channel. HTTP error code " + String(statusCode)); 
  }
  delay(15000);
}
