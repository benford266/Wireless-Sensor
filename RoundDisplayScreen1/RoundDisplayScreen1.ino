//
// Function: Tiny OLED ESP32 packages to display sensor data sent over ESP-NOW
// Author: BFGarage
// YT Channel: BFGarage
// Board: ESP32S OLED Packages 
//

#include <LovyanGFX.hpp>
#include <esp_now.h>
#include <WiFi.h>

// Screen configuration
class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_GC9A01 _panel_instance;
  lgfx::Bus_SPI _bus_instance;

public:
  LGFX(void) {
    {
      auto bus_cfg = _bus_instance.config();
      bus_cfg.spi_host = SPI2_HOST;
      bus_cfg.spi_mode = 0;
      bus_cfg.freq_write = 80000000;
      bus_cfg.freq_read = 20000000;
      bus_cfg.spi_3wire = true;
      bus_cfg.use_lock = true;
      bus_cfg.dma_channel = SPI_DMA_CH_AUTO;
      bus_cfg.pin_sclk = 6;
      bus_cfg.pin_mosi = 7;
      bus_cfg.pin_miso = -1;
      bus_cfg.pin_dc = 2;
      _bus_instance.config(bus_cfg);
      _panel_instance.setBus(&_bus_instance);
    }
    {
      auto panel_cfg = _panel_instance.config();
      panel_cfg.pin_cs = 10;
      panel_cfg.pin_rst = -1;
      panel_cfg.pin_busy = -1;
      panel_cfg.memory_width = 240;
      panel_cfg.memory_height = 240;
      panel_cfg.panel_width = 240;
      panel_cfg.panel_height = 240;
      panel_cfg.offset_x = 0;
      panel_cfg.offset_y = 0;
      panel_cfg.offset_rotation = 0;
      panel_cfg.dummy_read_pixel = 8;
      panel_cfg.dummy_read_bits = 1;
      panel_cfg.readable = false;
      panel_cfg.invert = true;
      panel_cfg.rgb_order = false;
      panel_cfg.dlen_16bit = false;
      panel_cfg.bus_shared = false;
      _panel_instance.config(panel_cfg);
    }
    setPanel(&_panel_instance);
  }
};

// Screen initiate 
LGFX display;

// Struct containing data received over ESP-NOW
typedef struct test_struct {
  int sensor1 = 999;
  int sensor2 = 999;
} test_struct;

// Create a struct_message called myData
test_struct myData;

// Copy data received from ESP-NOW to struct
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
}

// Setup device 
void setup(void) {
  // SPIConfig
  display.init();
  display.initDMA();
  display.startWrite();
  display.setColor(0, 0, 0);

  // Backlight config
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);

  // Logo screen
  display.setTextSize(3);
  display.drawString("BFGarage", 50, 110);
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

  // Register callback function to store the data 
  esp_now_register_recv_cb(OnDataRecv);
}

// Function to draw gauge
void drawGauge(int value) {
  display.startWrite();
  display.fillScreen(TFT_BLACK);

  // Draw gauge background
  display.fillCircle(120, 120, 100, TFT_DARKGREY);
  display.fillCircle(120, 120, 90, TFT_BLACK);

  // Draw gauge ticks
  for (int i = -150; i <= 150; i += 30) {
    float angle = i * DEG_TO_RAD;
    int x0 = 120 + cos(angle) * 90;
    int y0 = 120 + sin(angle) * 90;
    int x1 = 120 + cos(angle) * 80;
    int y1 = 120 + sin(angle) * 80;
    display.drawLine(x0, y0, x1, y1, TFT_WHITE);
  }

  // Draw gauge needle
  float angle = map(value, 0, 100, -150, 150) * DEG_TO_RAD;
  int x = 120 + cos(angle) * 70;
  int y = 120 + sin(angle) * 70;
  display.drawLine(120, 120, x, y, TFT_RED);

  // Draw gauge value
  display.setTextColor(TFT_WHITE);
  display.setTextSize(3);
  display.setCursor(90, 180);
  display.print(value);
  display.print(" PSI");

  display.endWrite();
}

// Main loop
void loop(void) {
  // Change me to pick which sensor to display
  int sensorvalue = myData.sensor1;

  // Check if ESP-NOW has received data from sender
  if (sensorvalue == 999) {
    // Wait to receive data from sensor
    display.startWrite();
    display.setTextSize(1.5);
    display.drawString("By BFGarage", 65, 150);
    display.drawString(String(WiFi.macAddress()), 45, 180);
    display.setTextSize(2);
    display.drawString("Connecting.", 50, 110);
    delay(1000);
    display.drawString("Connecting..", 50, 110);
    delay(1000);
    display.drawString("Connecting...", 50, 110);
    delay(1000);
    display.clear();
    display.endWrite();
  } else if (sensorvalue < -20) {
    // If sensor reading is below -20 update screen with sensor unplugged
    display.setTextSize(2);
    display.drawString("Sensor Unplugged", 25, 110);
    delay(1000);
    display.clear();
  } else {
    drawGauge(sensorvalue);
    delay(1000);
  }
}
