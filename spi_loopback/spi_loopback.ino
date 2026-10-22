#include <SPI.h>

#define SPI_SCK_PIN  12
#define SPI_MISO_PIN 13
#define SPI_MOSI_PIN 11
#define SPI_CS_PIN   10

SPIClass customSPI(FSPI);
SPISettings spiSettings(1000000, MSBFIRST, SPI_MODE0);

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000);

  pinMode(SPI_CS_PIN, OUTPUT);
  digitalWrite(SPI_CS_PIN, HIGH);

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
  digitalWrite(SPI_CS_PIN, LOW);

  customSPI.beginTransaction(spiSettings);
  byte sentByte = 0xA5;
  byte receivedByte = customSPI.transfer(sentByte);
  customSPI.endTransaction();

  digitalWrite(SPI_CS_PIN, HIGH);

  Serial.print("Sent 0x");
  Serial.print(sentByte, HEX);
  Serial.print(", received 0x");
  if (receivedByte < 16) {
    Serial.print("0");
  }
  Serial.println(receivedByte, HEX);

  delay(1000);
}
