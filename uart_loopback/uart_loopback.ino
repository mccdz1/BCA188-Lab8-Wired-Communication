/*
 * Laboratory Activity 8: Wired Communication
 * Sketch 1: UART Loopback
 * 
 * Microcontroller: ESP32-S3-N16R8
 * Interface: UART (Serial2)
 *
 * Description:
 *   Sends "PING" through Serial2 (9600 baud, 8 data bits, no parity, 1 stop bit)
 *   once per second on GPIO 17 (TX). Every byte received on GPIO 18 (RX) is
 *   printed to the USB Serial Monitor (115200 baud).
 *
 * Wiring for test:
 *   Jumper wire connecting GPIO 17 (TX) directly to GPIO 18 (RX).
 */

#define UART_TX_PIN 17
#define UART_RX_PIN 18

// ESP32 Hardware UART instance 2
HardwareSerial SerialPort(2);

void setup() {
  // Initialize USB Serial for computer monitor output
  Serial.begin(115200);
  while (!Serial && millis() < 3000); // Wait for USB CDC enumeration if needed

  // Configure UART2 at 9600 baud, 8 data bits, no parity, 1 stop bit (SERIAL_8N1)
  SerialPort.begin(9600, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);

  Serial.println("==========================================");
  Serial.println("       ESP32-S3 UART Loopback Test        ");
  Serial.println("==========================================");
  Serial.print("TX Pin : GPIO "); Serial.println(UART_TX_PIN);
  Serial.print("RX Pin : GPIO "); Serial.println(UART_RX_PIN);
  Serial.println("Baud   : 9600 (8N1)");
  Serial.println("Status : Ready. Transmitting PING every 1s...");
  Serial.println("------------------------------------------");
}

void loop() {
  // Transmit "PING" through Serial2
  SerialPort.print("PING");

  // Brief delay to allow transmission and reception
  delay(50);

  // Read all bytes received on RX and print to Serial Monitor
  bool receivedAny = false;
  while (SerialPort.available() > 0) {
    char inByte = SerialPort.read();
    Serial.write(inByte);
    receivedAny = true;
  }

  if (receivedAny) {
    Serial.println(); // Newline after receiving the full PING word
  }

  delay(1000); // Repeat once per second
}
