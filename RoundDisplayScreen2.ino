//
// Function: Tiny OLED ESP32 packages to display sensor data sent over ESP-NOW
// Author: BFGarage
// YT Channel: BFGarage
// Board: ESP32S OLED Packages 
//


#include <LovyanGFX.hpp>
#include <esp_now.h>
#include <WiFi.h>

class LGFX : public lgfx::LGFX_Device
{

  lgfx::Panel_GC9A01 _panel_instance;

  lgfx::Bus_SPI _bus_instance;
// Screen configuration
public:
  LGFX(void)
  {
    {
      auto cfg = _bus_instance.config();

      
      cfg.spi_host = SPI2_HOST;
      cfg.spi_mode = 0;                  
      cfg.freq_write = 80000000;         
      cfg.freq_read = 20000000;          
      cfg.spi_3wire = true;              
      cfg.use_lock = true;               
      cfg.dma_channel = SPI_DMA_CH_AUTO; 
      cfg.pin_sclk = 6;  
      cfg.pin_mosi = 7;  
      cfg.pin_miso = -1; 
      cfg.pin_dc = 2;    
      _bus_instance.config(cfg);              
      _panel_instance.setBus(&_bus_instance); 
    }

    {                                     
      auto cfg = _panel_instance.config(); 

      cfg.pin_cs = 10;  
      cfg.pin_rst = -1;  
      cfg.pin_busy = -1; 
      cfg.memory_width = 240;   
      cfg.memory_height = 240;  
      cfg.panel_width = 240;   
      cfg.panel_height = 240;   
      cfg.offset_x = 0;         
      cfg.offset_y = 0;         
      cfg.offset_rotation = 0;  
      cfg.dummy_read_pixel = 8; 
      cfg.dummy_read_bits = 1; 
      cfg.readable = false;     
      cfg.invert = true;        
      cfg.rgb_order = false;    
      cfg.dlen_16bit = false;   
      cfg.bus_shared = false;   

      _panel_instance.config(cfg);
    }

    setPanel(&_panel_instance); 
  }
};


LGFX display;

// Struct containing data recieved over ESP-NOW
typedef struct test_struct {
  int sensor1 =999;
  int sensor2 =999;
} test_struct;

// Create a struct_message called myData
test_struct myData;


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("S1 Pressure: ");
  Serial.println(myData.sensor1);
  Serial.println();
  Serial.print("S2 Pressure: ");
  Serial.println(myData.sensor2);
  Serial.println();
}

void setup(void)
{
  // SPIConfig
  display.init();
  display.initDMA();
  display.startWrite();
  display.setColor(0, 0, 0);

  //Backlight config
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  
  // Logo screen
  display.setTextSize(3);
  display.drawString("BFGarage",50,110);
  delay(5000);
  display.clear();

  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

}
uint32_t count = ~0;
void loop(void)
{
  // Change me to pick which sensor to display
  int sensorvalue = myData.sensor2;
  
  // Check if ESP-NOW has recieved data from sender
  if (sensorvalue == 999){
    // Wait to recieve data from sensor
    display.startWrite();
    display.setTextSize(1.5);
    display.drawString("By BFGarage",65,150);
    display.drawString(String(WiFi.macAddress()),45,180);
    display.setTextSize(2);
    display.drawString("Connecting.",50,110);
    delay(1000);
    display.drawString("Connecting..",50,110);
    delay(1000);
    display.drawString("Connecting...",50,110);
    delay(1000);
    display.clear();
    display.endWrite();
  }
  else
  {
    // Update screen with pressure data
    display.setTextSize(7);
    display.drawString(String(sensorvalue),60,90);
    display.setTextSize(4);
    display.drawString("PSI",80,170);
    delay(1000);
    display.clear();
    
  }

  

}
