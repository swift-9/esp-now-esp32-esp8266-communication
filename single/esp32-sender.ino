#include <WiFi.h>
#include <esp_now.h>

// Receiver MAC address (replace with your ESP8266’s MAC)
uint8_t receiverMAC[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Replace your mac address

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);

  // Set device as Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register send callback
  esp_now_register_send_cb(OnDataSent);

  // Add peer (the ESP8266)
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("ESP32 ready to send data...");
}

void loop() {
  const char *message = "Hello from ESP32!";
  esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)message, strlen(message));

  if (result == ESP_OK) {
    Serial.println("Sent successfully");
  } else {
    Serial.println("Error sending data");
  }

  delay(2000);
}
