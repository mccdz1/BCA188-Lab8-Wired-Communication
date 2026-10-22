#include <Wire.h>

#define I2C_SDA_PIN 8
#define I2C_SCL_PIN 9

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  Serial.println("==========================================");
  Serial.println("        ESP32-S3 I2C Bus Scanner          ");
  Serial.println("==========================================");
  Serial.print("SDA Pin : GPIO "); Serial.println(I2C_SDA_PIN);
  Serial.print("SCL Pin : GPIO "); Serial.println(I2C_SCL_PIN);
  Serial.println("Scanning range: 0x01 to 0x7E...\n");

  byte error, address;
  int nDevices = 0;

  for (address = 0x01; address <= 0x7E; address++) {
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
}
