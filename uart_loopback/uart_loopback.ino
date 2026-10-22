#define UART_TX_PIN 17
#define UART_RX_PIN 18

HardwareSerial SerialPort(2);

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000);

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
  SerialPort.print("PING");

  delay(50);

  bool receivedAny = false;
  while (SerialPort.available() > 0) {
    char inByte = SerialPort.read();
    Serial.write(inByte);
    receivedAny = true;
  }

  if (receivedAny) {
    Serial.println();
  }

  delay(1000);
}
