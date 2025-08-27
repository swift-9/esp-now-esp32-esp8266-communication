#include <WiFi.h> // For esp32
//#include <ESP8266WiFi.h> For esp8266
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
}
void loop() {}
