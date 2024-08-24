#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "RoboR";  // Enter your SSID here
const char* password = "mylittlewifi";  //Enter your Password here

WebServer server(80);

String header;

IPAddress local_IP(192, 168, 31, 200);
IPAddress gateway(192, 168, 31, 1);
IPAddress subnet(255, 255, 0, 0);

String HTML = "<!DOCTYPE html>\
<html>\
<body>\
<h1>My First Web Server with ESP32 - Station Mode POG &#128522;</h1>\
<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>\
</body>\
</html>";

void handle_root() {
  server.send(200, "text/html", HTML);
}

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to");
  Serial.println(ssid);

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure IP config");
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  
  Serial.println("\nConnection success");
  Serial.print("Device IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_root);
  server.begin();
  Serial.println("Server started");
  delay(100);
}


void loop() {
  server.handleClient();
} 

// 192.168.31.209