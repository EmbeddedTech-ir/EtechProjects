#include <Arduino.h>
#include <Wire.h>
#include <vl53l7cx_class.h>

// --- Pin Definitions ---
#define LPN_PIN       -1 
#define I2C_RST_PIN   A1 

// --- Configuration ---
#define SENSOR_I2C_ADDR  0x29      
#define SERIAL_BAUD      921600    
#define I2C_SPEED        400000   

VL53L7CX sensor(&Wire1, LPN_PIN, I2C_RST_PIN);

// Global variable to track current zones (16 or 64)
int zones_per_frame = 16; 

void hardwareReset() {
  digitalWrite(I2C_RST_PIN, HIGH);
  delay(100);
  digitalWrite(I2C_RST_PIN, LOW);
  delay(100); 
}

bool checkI2CConnection() {
  Wire1.beginTransmission(SENSOR_I2C_ADDR);
  return (Wire1.endTransmission() == 0);
}

// Helper to switch resolution dynamically
void setResolution(uint8_t mode) {
  sensor.vl53l7cx_stop_ranging();
  
  if (mode == 4) {
    sensor.vl53l7cx_set_resolution(VL53L7CX_RESOLUTION_4X4);
    // 4x4 is very sensitive, can run fast (60Hz)
    sensor.vl53l7cx_set_ranging_frequency_hz(60);
    zones_per_frame = 16;
    Serial.println("STATUS: Mode 4x4 (60Hz)");
  } 
  else if (mode == 8) {
    sensor.vl53l7cx_set_resolution(VL53L7CX_RESOLUTION_8X8);
    // 8x8 splits light more, so we lower speed to 15Hz to get better accuracy
    sensor.vl53l7cx_set_ranging_frequency_hz(15); 
    zones_per_frame = 64;
    Serial.println("STATUS: Mode 8x8 (15Hz)");
  }
  
  sensor.vl53l7cx_start_ranging();
}

void setup()
{
  Serial.begin(SERIAL_BAUD);
  while(!Serial);

  pinMode(I2C_RST_PIN, OUTPUT);
  pinMode(LPN_PIN, OUTPUT);

  Wire1.begin();
  Wire1.setClock(I2C_SPEED);

  Serial.println("STATUS: System Started. Searching for sensor...");

  while (true) {
    hardwareReset();
    delay(2000); 
    if (checkI2CConnection()) {
      Serial.println("STATUS: Sensor FOUND at 0x29!");
      break; 
    }
    Serial.println("ERROR: Sensor not found. Retrying in 1 sec...");

  }

  Serial.println("STATUS: Loading Firmware...");
  
  if (sensor.begin() != 0) {
    Serial.println("ERROR: sensor.begin() failed!");
    while (1) { delay(100); } 
  }

  if (sensor.init_sensor() != 0) {
    Serial.println("ERROR: sensor.init_sensor() failed!");
    while (1) { delay(100); }
  }

  // Start with default 8x8
  setResolution(8);
}

void loop()
{
  // 1. Check for incoming commands from Python
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == '4') setResolution(4);
    if (cmd == '8') setResolution(8);
  }

  // 2. Read Sensor Data
  VL53L7CX_ResultsData results;
  uint8_t ready = 0;

  sensor.vl53l7cx_check_data_ready(&ready);

  if (ready)
  {
    sensor.vl53l7cx_get_ranging_data(&results);

    // Loop changes based on zones_per_frame (16 or 64)
    for (int i = 0; i < zones_per_frame; i++) 
    {
      long distance = results.distance_mm[i * VL53L7CX_NB_TARGET_PER_ZONE];
      uint8_t status = results.target_status[i * VL53L7CX_NB_TARGET_PER_ZONE];
      uint8_t targets = results.nb_target_detected[i];

      if (targets > 0 && (status == 5 || status == 9)) {
        Serial.print(distance);
      } else {
        Serial.print("2000"); 
      }
      if (i < zones_per_frame - 1) Serial.print(",");
    }
    Serial.println();
  }
}
