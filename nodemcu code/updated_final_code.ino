#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h> 
#include <Wire.h>

#define DHTPIN D7
#define DHTTYPE DHT22
#define motionSensorPin D6
#define gpsRxPin D1
#define gpsTxPin D2
#define ena D5
#define in1 D4 // Changed to D4
#define in2 D3 // Changed to D3
#define enb D0
#define in3 D8
#define in4 D10

SoftwareSerial neo6m(gpsTxPin, gpsRxPin);
TinyGPSPlus gps;

const char* ssid = "Thejandeera's S21 Ultra";
const char* password = "87654321";
const char* serverAddress = "http://192.168.46.115:5000/api/tempSendRcv/temperature";
String GMAP_API_KEY = "AIzaSyDJAyFD44yTrJ9oQeGksP1bw290KKnAxc0";

WiFiServer server(80);
String html;

DHT dht(DHTPIN, DHTTYPE);
float previousTemperature = -999; // Initialize with an unlikely value
bool previousMotionStatus = false;

void setup() {
  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(9600);
  pinMode(motionSensorPin, INPUT);
  dht.begin();
  neo6m.begin(9600);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  mpower(1,1,255);
  mpower(2,1,255);

}

void mpower(int motor, int rotation, int spd){
  int pwm;
  int pa;
  int pb;
  if(motor == 1){
    pwm = ena;
    pa = in1;
    pb = in2;
  }
  else if (motor == 2){
    pwm = enb;
    pa = in3;
    pb = in4;
  }
  else{
    return;
  }
  if(rotation == 0){
    digitalWrite(pa,HIGH);
    digitalWrite(pb,LOW);
  }
  else if (rotation == 1){
    digitalWrite(pa,HIGH);
    digitalWrite(pb,LOW);
  }
  else if (rotation == 2){
    digitalWrite(pb,HIGH);
    digitalWrite(pa,LOW);
  }
  analogWrite(pwm,spd);
}

void loop() {
  delay(1000);
  float temperature = dht.readTemperature();

  if (isnan(temperature)) {
    Serial.println("Failed to read temperature from DHT sensor");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");  
  int motionDetected = digitalRead(motionSensorPin);
  bool currentMotionStatus = (motionDetected == HIGH);
  
  Serial.print("Motion sensor status: ");
  Serial.println(currentMotionStatus ? "Motion detected!" : "No motion detected.");

  if (temperature != previousTemperature || currentMotionStatus  != previousMotionStatus ){
    WiFiClient client;
    HTTPClient http;
    http.begin(client, serverAddress); 
    
    http.addHeader("Content-Type", "application/json");
    
    String payload = "{\"temperature\": " + String(temperature) + ", \"motion\": " + String(currentMotionStatus ? "true" : "false") + "}";
    
    int httpResponseCode = http.POST(payload);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response Code: " + String(httpResponseCode));
      Serial.println(response);
       
    } else {
      Serial.println("Error sending data to server"); 
    }
    
    http.end();
  }

  smartdelay_gps(1000);
  
  if (gps.location.isValid()) 
  {
    
    String latitude = String(gps.location.lat(), 6);
    String longitude = String(gps.location.lng(), 6); 
    
    WiFiClient client = server.available();
    if(client) {                             
    Serial.println("new client");          
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();          
          if (c == '\n') {                     
          if (currentLine.length() == 0) {  
      
    html="<!DOCTYPE html>";
    html+="<html lang='en'>";
    html+="<head>";
    html+="<meta charset='UTF-8'>";
    html+="<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html+="<meta http-equiv='X-UA-Compatible' content='ie=edge'>";
    html+="<title>My Google Map</title>";
    html+="<style>#map{height:400px;width:100%;}</style>";
    html+="</head>";
    html+="<body>";
    html+="<h1>ATCS Google Map</h1>";
    html+="<div id='map'></div>";
    html+="<script>";
    html+="var map;";
    html+="var marker;";
    
    html+="var INTERVAL = 5000;";
    
   
    html+="function initMap(){";
      
      html+="var options = {";
          html+="zoom:16,";
          html+="center:{lat:"+latitude+",lng:"+longitude+"},";
          html+="mapTypeId: google.maps.MapTypeId.ROADMAP,";
      html+="};";
      
      html+="map = new google.maps.Map(document.getElementById('map'), options);";
    html+="}";
    html+="function getMarkers() {";
      
      html+="var newLatLng = new google.maps.LatLng("+latitude+", "+longitude+");";
      
      html+="marker = new google.maps.Marker({";
        html+="position: newLatLng,";
        html+="map: map";
      html+="});";
      
    html+="}";
    html+="window.setInterval(getMarkers,INTERVAL);";
    
    html+="</script>";
    html+="<script async defer src='https://maps.googleapis.com/maps/api/js?key="+GMAP_API_KEY+"&callback=initMap'>";
    html+="</script>";
    html+="</body></html>";
 
    client.print(html);
    client.println();
    
            break;
          } else {   currentLine = ""; }
        } else if (c != '\r') { 
          currentLine += c; 
        }
         
      }
    }
    
    client.stop();
    Serial.println("client disconnected");
    }    
  }
  else // If GPS is offline, send the last known location
  {
    String latitude = "YOUR_LAST_KNOWN_LATITUDE";
    String longitude = "YOUR_LAST_KNOWN_LONGITUDE";
    
    WiFiClient client = server.available();
    if(client) {                             
    Serial.println("new client");          
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();          
          if (c == '\n') {                     
          if (currentLine.length() == 0) {  
      
    html="<!DOCTYPE html>";
    html+="<html lang='en'>";
    html+="<head>";
    html+="<meta charset='UTF-8'>";
    html+="<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html+="<meta http-equiv='X-UA-Compatible' content='ie=edge'>";
    html+="<title>My Google Map</title>";
    html+="<style>#map{height:400px;width:100%;}</style>";
    html+="</head>";
    html+="<body>";
    html+="<h1>ATCS Google Map</h1>";
    html+="<div id='map'></div>";
    html+="<script>";
    html+="var map;";
    html+="var marker;";
    
    html+="var INTERVAL = 5000;";
    
   
    html+="function initMap(){";
      
      html+="var options = {";
          html+="zoom:16,";
          html+="center:{lat:"+latitude+",lng:"+longitude+"},";
          html+="mapTypeId: google.maps.MapTypeId.ROADMAP,";
      html+="};";
      
      html+="map = new google.maps.Map(document.getElementById('map'), options);";
    html+="}";
    html+="function getMarkers() {";
      
      html+="var newLatLng = new google.maps.LatLng("+latitude+", "+longitude+");";
      
      html+="marker = new google.maps.Marker({";
        html+="position: newLatLng,";
        html+="map: map";
      html+="});";
      
    html+="}";
    html+="window.setInterval(getMarkers,INTERVAL);";
    
    html+="</script>";
    html+="<script async defer src='https://maps.googleapis.com/maps/api/js?key="+GMAP_API_KEY+"&callback=initMap'>";
    html+="</script>";
    html+="</body></html>";
 
    client.print(html);
    client.println();
    
            break;
          } else {   currentLine = ""; }
        } else if (c != '\r') { 
          currentLine += c; 
        }
         
      }
    }
    
    client.stop();
    Serial.println("client disconnected");
    }    
  }

  previousTemperature = temperature;
  previousMotionStatus = currentMotionStatus;
}

static void smartdelay_gps(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (neo6m.available())
      gps.encode(neo6m.read());
  } while (millis() - start < ms);
}
