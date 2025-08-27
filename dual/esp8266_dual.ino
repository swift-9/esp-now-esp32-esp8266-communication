#include <ESP8266WiFi.h>
#include <espnow.h>

// ESP32 MAC address
uint8_t peerMac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Replace your mac address

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.printf("Sent to Esp32 | Status: %s\n",
                sendStatus == 0 ? "Success" : "Fail");
}

void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  Serial.print("From ESP32: ");
  for (int i = 0; i < len; i++) {
    Serial.print((char)incomingData[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Add ESP32 as peer
  esp_now_add_peer(peerMac, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

  Serial.println("ESP8266 ready. Talking to ESP32...");
}

void loop() {
  const char *msg = "Hello from ESP8266!";
  esp_now_send(peerMac, (uint8_t *)msg, strlen(msg));
  delay(3000);
}
