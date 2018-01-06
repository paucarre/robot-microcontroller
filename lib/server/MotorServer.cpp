#include "Arduino.h"

#include <MotorServer.h>

MotorServer::MotorServer() {
}

void MotorServer::initialize(WiFiServer& server)
{
  IPAddress ip(192, 168, 0, 130);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress dns(89, 101, 160, 4);
  WiFi.config(ip, gateway, subnet, dns);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
  server.begin();
  server.setNoDelay(true);
 }
