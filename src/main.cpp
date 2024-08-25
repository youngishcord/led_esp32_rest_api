#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include "ESPAsyncWebServer.h"
#include <ArduinoJson.h>

#define RELE_PIN 13
#define LED_PIN 12

const char* ssid = "RoboR";  // Enter your SSID here
const char* password = "mylittlewifi";  //Enter your Password here

AsyncWebServer server(80);

String header;

IPAddress local_IP(192, 168, 31, 200);
IPAddress gateway(192, 168, 31, 1);
IPAddress subnet(255, 255, 0, 0);

bool state = false;

String ledState;

String currentColor = "#00ff00";
int brightnes = 40;


String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(LED_PIN)){
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
  pinMode(RELE_PIN, OUTPUT);

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
    request->send(SPIFFS, "/colorPalete/index.html", String(), false, processor);
  });
  // server.on("/css/styles.css", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send(SPIFFS, "/colorPalete/css/styles.css", "text/css");
  // });
  // server.on("/js/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send(SPIFFS, "/colorPalete/js/script.js", "application/javascript");
  // });

  server.on("/:color", HTTP_POST, [](AsyncWebServerRequest *request) {
    // AsyncWebParameter* result = request->getParam("color");
    Serial.println(123);
  });

  server.on("/getCurentState", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("call get current state");
    JsonDocument json;

    json["state"] = state;
    json["color"] = currentColor;
    json["brightnes"] = brightnes;

    String message;
    serializeJson(json, message);
    request->send(200, "application/json", message);
  });

  server.on("/get_color", HTTP_GET, [](AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam("color")) {
            message = request->getParam("color")->value();
            currentColor = "#" + message;
        } else {
            message = "no result";
        }
        request->send(200);
        Serial.println(currentColor);
    });

  server.on("/update_state", HTTP_POST, [](AsyncWebServerRequest * request){}, 
  NULL, [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    String mess = "";
    for (size_t i = 0; i < len; i++) {
      mess += (char)data[i];
    }

    JsonDocument json;
    DeserializationError error = deserializeJson(json, mess);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    Serial.println(mess);

    state = json["state"];
    const char* _color = json["color"];
    currentColor = String(_color);
    brightnes = json["brightnes"];

    Serial.println(state);
    Serial.println(currentColor);
    Serial.println(brightnes);

    digitalWrite(RELE_PIN, state);

    request->send(200);
  });

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");
  
  server.serveStatic("/", SPIFFS, "/colorPalete").setDefaultFile("index.html");

  server.begin();
  Serial.println("Server started");
  
  delay(100);
}


void loop() {

} 
