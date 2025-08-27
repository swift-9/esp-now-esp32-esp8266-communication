#include <WiFi.h>
#include <esp_now.h>

// Replace with ESP8266 MAC address
uint8_t peer_esp8266[] = {0xCC, 0x50, 0xE3, 0x6B, 0x9C, 0x0F};

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.printf("Last Packet Sent to: ESP8266 | Status: %s\n",
                status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

// Callback when data is received
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int len) {
  Serial.print("From ESP8266: ");
  for (int i = 0; i < len; i++) {
    Serial.print((char)data[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callbacks
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Add ESP8266 as peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peer_esp8266, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add ESP8266 peer");
    return;
  }

  Serial.println("ESP32 ready. Talking to ESP8266...");
}

void loop() {

  const char *msg = "Hello from ESP32!";
  esp_now_send(peer_esp8266, (uint8_t *)msg, strlen(msg));
  delay(3000); // send every 3s
}
