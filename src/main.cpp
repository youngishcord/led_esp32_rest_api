#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include "ESPAsyncWebServer.h"
#include <ArduinoJson.h>
#include <FastLED.h>

#define LEDS_PIN 13
#define NUM_LEDS 120
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

const char* ssid = "RoboR";  // Enter your SSID here
const char* password = "mylittlewifi";  //Enter your Password here

AsyncWebServer server(80);

String header;

IPAddress local_IP(192, 168, 31, 200);
IPAddress gateway(192, 168, 31, 1);
IPAddress subnet(255, 255, 0, 0);


bool state = false;
String currentColor = "#000000";
int brightness = 0;


void setAllLedColor(CRGB color) {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
}

CRGB hexStringToColor(const String& hexString) {
    String cleanHexString = hexString;
    cleanHexString.toUpperCase();
    if (cleanHexString.startsWith("#")) {
        cleanHexString = cleanHexString.substring(1);
    } else if (cleanHexString.startsWith("0x")) {
        cleanHexString = cleanHexString.substring(2);
    }
    
    uint32_t color = (uint32_t) strtol(cleanHexString.c_str(), NULL, 16);
    
    return CRGB((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
}


void setup() {
  delay(1000);

  FastLED.addLeds<WS2812B, LEDS_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
  FastLED.clear();
  FastLED.show();

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
    request->send(SPIFFS, "/colorPalete/index.html", String(), false);
  });
  // server.on("/css/styles.css", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send(SPIFFS, "/colorPalete/css/styles.css", "text/css");
  // });
  // server.on("/js/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send(SPIFFS, "/colorPalete/js/script.js", "application/javascript");
  // });

  server.on("/:color", HTTP_POST, [](AsyncWebServerRequest *request) {
    // AsyncWebParameter* result = request->getParam("color");
    // Serial.println(123);
  });

  server.on("/getCurentState", HTTP_GET, [](AsyncWebServerRequest *request) {
    // Serial.println("call get current state");
    JsonDocument json;

    json["state"] = state;
    json["color"] = currentColor;
    json["brightness"] = brightness;

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
        // Serial.println(currentColor);
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

    // Serial.println(mess);

    state = json["state"];
    const char* _color = json["color"];
    currentColor = String(_color);
    brightness = json["brightness"];

    // Serial.println(state);
    // Serial.println(currentColor);
    // Serial.println(brightness);

    if (state) {
      // Serial.println("vistalau cvet");
      setAllLedColor(hexStringToColor(currentColor));
      FastLED.setBrightness(brightness);
    } else {
      // Serial.println("отключаю ленту");
      FastLED.clear();
    }
    FastLED.show();

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
