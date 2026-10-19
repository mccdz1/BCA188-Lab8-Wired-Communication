/*
 * Laboratory Activity 8: Wired Communication
 * Sketch 2: I2C Scanner
 * 
 * Microcontroller: ESP32-S3-N16R8
 * Interface: I2C (Wire)
 *
 * Description:
 *   Initializes the I2C bus on GPIO 8 (SDA) and GPIO 9 (SCL).
 *   Scans every 7-bit address from 0x01 to 0x7E once at startup.
 *   Prints "Found device at 0x.." for each response, or
 *   "No I2C devices found" if no slave device acknowledges.
 *
 * Wiring for test:
 *   I2C Module (e.g., SSD1306 OLED or BMP280/BME280)
 *   VCC -> ESP32 3V3
 *   GND -> ESP32 GND
 *   SDA -> ESP32 GPIO 8
 *   SCL -> ESP32 GPIO 9
 */

#include <Wire.h>

#define I2C_SDA_PIN 8
#define I2C_SCL_PIN 9

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000); // Wait for USB CDC enumeration if needed

  // Initialize I2C bus with ESP32-S3 custom SDA and SCL pins
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  Serial.println("==========================================");
  Serial.println("        ESP32-S3 I2C Bus Scanner          ");
  Serial.println("==========================================");
  Serial.print("SDA Pin : GPIO "); Serial.println(I2C_SDA_PIN);
  Serial.print("SCL Pin : GPIO "); Serial.println(I2C_SCL_PIN);
  Serial.println("Scanning range: 0x01 to 0x7E...");
  Serial.println("------------------------------------------");

  byte error, address;
  int nDevices = 0;

  for (address = 0x01; address <= 0x7E; address++) {
    // Attempt transmission to address
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Found device at 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }

  Serial.println("------------------------------------------");
  if (nDevices == 0) {
    Serial.println("No I2C devices found");
  } else {
    Serial.print("Scan complete. Devices found: ");
    Serial.println(nDevices);
  }
  Serial.println("==========================================");
}

void loop() {
  // Lab specification requires scanning once at startup.
  // To scan again, press the EN / RST button on the board.
}
