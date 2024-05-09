#include <esp_now.h>
#include <WiFi.h>
// ESP Board MAC Address:  8C:AA:B5:94:E3:94
// REPLACE WITH YOUR ESP RECEIVER'S MAC ADDRESS
uint8_t broadcastAddress1[] = {0x68, 0x67, 0x25, 0x6A, 0x51, 0x70};
uint8_t broadcastAddress2[] = {0x68, 0x67, 0x25, 0x6A, 0x51, 0x70};
typedef struct test_struct {
  int sensor1;
  int sensor2;
} test_struct;

test_struct test;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Sensor config
float inputP1;
float inputP2;
int outputP1;
int sensor1PSI;
int sensor2PSI;
int Sensor1MaxPSI = 300; // Max pressure sensor can read
int Sensor2MaxPSI = 300; // Max pressure sensor can read
int SensorMinRes = 416; // 0.5v value of 12 bit ADC read
int SensorMaxRes = 3750; // 4.5 value of 12 bit ADC read
int SensorRangeRes = SensorMaxRes - SensorMinRes; // Range of values between 0.5v and 4.5v 


void setup() {
  Serial.begin(115200);
 
  WiFi.mode(WIFI_STA);
 
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_send_cb(OnDataSent);
   
  // register peer
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  // register first peer  
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
}
}
 
void loop() {

  // Read value from ADC
  inputP1 = analogRead(A12); //12 Bit 
  inputP2 = analogRead(A13); //12 Bit 
  // Maths to ADC vaule into PSI Unit
  sensor1PSI = ((inputP1 - SensorMinRes)* Sensor1MaxPSI /SensorRangeRes);
  sensor2PSI = ((inputP2 - SensorMinRes)* Sensor2MaxPSI /SensorRangeRes);
  // Put sensor data into var
  test.sensor1 = sensor1PSI;
  test.sensor2 = sensor2PSI;
  // Send sensor data over ESP NOW
  esp_err_t result = esp_now_send(0, (uint8_t *) &test, sizeof(test_struct));
  // Data send delay
  delay(1000);
}
