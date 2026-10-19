/*
 * Laboratory Activity 8: Wired Communication
 * Sketch 3: SPI Loopback
 * 
 * Microcontroller: ESP32-S3-N16R8
 * Interface: SPI (FSPI)
 *
 * Description:
 *   Initializes SPI on GPIO 12 (SCK), GPIO 13 (MISO), GPIO 11 (MOSI),
 *   and GPIO 10 (CS) at 1 MHz, Most-Significant-Bit first, SPI Mode 0.
 *   Once per second, it pulls CS LOW, transmits byte 0xA5, pulls CS HIGH,
 *   and prints "Sent 0xA5, received 0x.." to the USB Serial Monitor.
 *
 * Wiring for test:
 *   Jumper wire connecting GPIO 11 (MOSI) directly to GPIO 13 (MISO).
 */

#include <SPI.h>

#define SPI_SCK_PIN  12
#define SPI_MISO_PIN 13
#define SPI_MOSI_PIN 11
#define SPI_CS_PIN   10

// SPI instance and settings: 1 MHz, MSBFIRST, SPI Mode 0
SPIClass customSPI(FSPI);
SPISettings spiSettings(1000000, MSBFIRST, SPI_MODE0);

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000); // Wait for USB CDC enumeration if needed

  // Configure Chip Select (CS) pin as output, start HIGH (inactive)
  pinMode(SPI_CS_PIN, OUTPUT);
  digitalWrite(SPI_CS_PIN, HIGH);

  // Initialize SPI bus with custom pin mapping
  customSPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SPI_CS_PIN);

  Serial.println("==========================================");
  Serial.println("        ESP32-S3 SPI Loopback Test        ");
  Serial.println("==========================================");
  Serial.print("SCK  : GPIO "); Serial.println(SPI_SCK_PIN);
  Serial.print("MOSI : GPIO "); Serial.println(SPI_MOSI_PIN);
  Serial.print("MISO : GPIO "); Serial.println(SPI_MISO_PIN);
  Serial.print("CS   : GPIO "); Serial.println(SPI_CS_PIN);
  Serial.println("Speed: 1 MHz | MSBFIRST | SPI Mode 0");
  Serial.println("Wiring: Connect GPIO 11 (MOSI) to GPIO 13 (MISO)");
  Serial.println("------------------------------------------");
}

void loop() {
  // Step 1: Pull CS LOW to begin SPI transfer frame
  digitalWrite(SPI_CS_PIN, LOW);

  // Step 2: Begin transaction & transfer byte 0xA5
  customSPI.beginTransaction(spiSettings);
  byte sentByte = 0xA5;
  byte receivedByte = customSPI.transfer(sentByte);
  customSPI.endTransaction();

  // Step 3: Pull CS HIGH to finish SPI transfer frame
  digitalWrite(SPI_CS_PIN, HIGH);

  // Step 4: Print results in hex format
  Serial.print("Sent 0x");
  Serial.print(sentByte, HEX);
  Serial.print(", received 0x");
  if (receivedByte < 16) {
    Serial.print("0");
  }
  Serial.println(receivedByte, HEX);

  delay(1000); // Repeat once per second
}
