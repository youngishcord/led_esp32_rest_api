#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include "ESPAsyncWebServer.h"

const char* ssid = "RoboR";  // Enter your SSID here
const char* password = "mylittlewifi";  //Enter your Password here

AsyncWebServer server(80);

String header;

IPAddress local_IP(192, 168, 31, 200);
IPAddress gateway(192, 168, 31, 1);
IPAddress subnet(255, 255, 0, 0);

const int ledPin = 2;
String ledState;

const char* PARAM_MESSAGE = "color";


String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "ON";
    }
    else{
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }
  return String();
}


void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure IP config");
    return;
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  
  Serial.println("\nConnection success");
  Serial.print("Device IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/color_palete/index.html", String(), false, processor);
  });

  server.on("/:color", HTTP_POST, [](AsyncWebServerRequest *request) {
    // AsyncWebParameter* result = request->getParam("color");
    Serial.println(123);
  });

  server.on("/:a", HTTP_GET, [](AsyncWebServerRequest *request) {
    // AsyncWebParameter* result = request->getParam("color");
    Serial.println("qwert");
  });

  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        request->send(200);
        Serial.println(message);
    });

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");
  
  // server.enableCORS(true);
  // server.serveStatic()
  server.begin();
  Serial.println("Server started");
  
  delay(100);
}


void loop() {

} 
