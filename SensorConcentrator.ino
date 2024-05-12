//
// Function: Sensor concentrator that uses ESP-NOW to send data to ESP OLED packages
// Author: BFGarage
// YT Channel: BFGarage
// Board: ESP32s Dev Board
//


#include <esp_now.h>
#include <WiFi.h>

// REPLACE with Screen 1 MAC address
uint8_t broadcastAddress1[] = {0x68, 0x67, 0x25, 0x6A, 0x51, 0x70};
// REPLACE with Screen 2 MAC address
uint8_t broadcastAddress2[] = {0x68, 0x67, 0x25, 0x6A, 0x51, 0x70};

// Struct containing data sent over ESP-NOW
typedef struct test_struct {
  int sensor1;
  int sensor2;
} test_struct;

// Create a struct_message called myData
test_struct test;

// Something not sure 
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
}

// Sensor config
float inputP1;
float inputP2;
int outputP1;
int sensor1PSI;
int sensor2PSI;
int Sensor1MaxPSI = 300; // Sensor 1 Max pressure 
int Sensor2MaxPSI = 300; // Sensor 2 Max pressure 
int SensorMinRes = 416; // 0.5v value of 12 bit ADC read
int SensorMaxRes = 3750; // 4.5 value of 12 bit ADC read
int SensorRangeRes = SensorMaxRes - SensorMinRes; // Range of values between 0.5v and 4.5v 

// Setup device 
void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
 
 // ESP-NOW init 
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // cb to check ESP-NOW is init
  esp_now_register_send_cb(OnDataSent);
   
  // Register peer
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Register screen 1
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register screen 2 
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
}
}
 
void loop() {

  // Read sensor value from ADC
  inputP1 = analogRead(12); //12 Bit 
  inputP2 = analogRead(13); //12 Bit 
  
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
