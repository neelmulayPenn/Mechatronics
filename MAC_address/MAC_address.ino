#include <WiFi.h>

void setup() {
  Serial.begin(115200);

  // Set Wi-Fi mode to Station
  WiFi.mode(WIFI_STA);

  // Get and print the MAC address
  Serial.print("ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  // Nothing to do here
}
