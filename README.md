# Laboratory Activity 8: Wired Communication

**Course:** BCA188 (AY26-27 JFV)  
**Task:** Verify the three wired interfaces on the ESP32 board: UART, $\text{I}^2\text{C}$, and SPI.  
**Microcontroller Target:** ESP32-S3-N16R8 (16MB Octal Flash, 8MB Octal PSRAM)  
**Prototyping Medium:** Solderless Breadboard  

---

## 1. Materials Used

- **Microcontroller:** ESP32-S3-N16R8 Development Board
- **Breadboard:** Standard Full-Size Solderless Breadboard
- **Cables & Jumpers:** USB Type-C Data Cable, Male-to-Male (M-M) jumper wires
- **$\text{I}^2\text{C}$ Peripheral:** 0.96" SSD1306 OLED Display (Address `0x3C`) / BMP280 Sensor (Address `0x76`)
- **Software:** Arduino IDE with ESP32 Board Core (Espressif Systems)

---

## 2. Pin Table

All GPIOs were selected to avoid internal Octal SPI Flash/PSRAM lines (GPIO 33–37) and strapping pins (GPIO 0, 45, 46).

| Signal | GPIO Used | Function / Test Connection | Notes |
| :--- | :---: | :--- | :--- |
| **UART TX** | **GPIO 17** | Serial2 TX $\rightarrow$ Jumper to RX | Output to loopback line |
| **UART RX** | **GPIO 18** | Serial2 RX $\leftarrow$ Jumper from TX | Input from loopback line |
| **$\text{I}^2\text{C}$ SDA** | **GPIO 8** | $\text{I}^2\text{C}$ Serial Data $\rightarrow$ Module SDA | Open-drain data line |
| **$\text{I}^2\text{C}$ SCL** | **GPIO 9** | $\text{I}^2\text{C}$ Serial Clock $\rightarrow$ Module SCL | Clock line |
| **SPI SCK** | **GPIO 12** | SPI Serial Clock | Bus clock (1 MHz) |
| **SPI MOSI** | **GPIO 11** | Master Out Slave In $\rightarrow$ Jumper to MISO | Output to loopback line |
| **SPI MISO** | **GPIO 13** | Master In Slave Out $\leftarrow$ Jumper from MOSI | Input from loopback line |
| **SPI CS** | **GPIO 10** | SPI Chip Select (Active LOW) | Bus control |

---

## 3. Wiring Diagram & Breadboard Setup

### Schematic Layout

```text
                  ESP32-S3-N16R8 Breadboard Wiring
               +--------------------------------------+
               |            [USB-C PORT]              |
               |                                      |
   UART Test:  | [GPIO 17] (TX) -----\ (Jumper Wire)  |
               | [GPIO 18] (RX) <----/                |
               |                                      |
   I2C Module: | [3V3]     ----------> VCC (I2C Mod)  |
               | [GND]     ----------> GND (I2C Mod)  |
               | [GPIO 8]  ----------> SDA (I2C Mod)  |
               | [GPIO 9]  ----------> SCL (I2C Mod)  |
               |                                      |
   SPI Test:   | [GPIO 10] ----------> CS (Active L)  |
               | [GPIO 12] ----------> SCK            |
               | [GPIO 11] (MOSI) ----\ (Jumper Wire) |
               | [GPIO 13] (MISO) <---/               |
               +--------------------------------------+
```

### Circuit Photo
*(Replace placeholder below with your photo: `assets/circuit_setup.jpg`)*

![Breadboard Circuit Setup](assets/circuit_setup.jpg)

---

## 4. Sketches Overview

This repository is organized into three folders:

1. **`uart_loopback/uart_loopback.ino`**:
   Configures hardware `Serial2` at 9600 baud (8N1) on GPIO 17 (TX) and GPIO 18 (RX). Sends `"PING"` once per second and prints every received byte to the USB Serial Monitor at 115200 baud.
2. **`i2c_scanner/i2c_scanner.ino`**:
   Initializes the `Wire` library on GPIO 8 (SDA) and GPIO 9 (SCL). Scans 7-bit addresses `0x01` through `0x7E` once at boot and reports detected device addresses.
3. **`spi_loopback/spi_loopback.ino`**:
   Configures SPI bus at 1 MHz, MSBFIRST, SPI Mode 0 using GPIO 12 (SCK), GPIO 13 (MISO), GPIO 11 (MOSI), and GPIO 10 (CS). Transmits `0xA5` while CS is LOW and prints the received byte in hex.

---

## 5. Expected versus Observed Results

| Test | Expected Output | Observed Output | Status |
| :--- | :--- | :--- | :---: |
| **UART with jumper** | Prints `PING` once every second | Echoes `PING` every second | PASS |
| **UART without jumper** | No characters received / transmission silent | Output stops; no characters received | PASS |
| **$\text{I}^2\text{C}$ with module** | `Found device at 0x3C` *(or `0x76`)* | `Found device at 0x3C` | PASS |
| **$\text{I}^2\text{C}$ without module** | `No I2C devices found` | `No I2C devices found` | PASS |
| **SPI with jumper** | `Sent 0xA5, received 0xA5` | `Sent 0xA5, received 0xA5` | PASS |
| **SPI without jumper** | `Sent 0xA5, received 0x00` *(or `0xFF`)* | `Sent 0xA5, received 0x00` | PASS |
| **Sketch at 9600, monitor at 115200** | Garbled / corrupt glyphs (`?` / gibberish) | Corrupted characters / unreadable symbols | PASS |
| **Sketch at 9600, monitor at 9600** | Clean, readable text output | Clear, readable text output | PASS |

---

## 6. Serial Monitor Outputs

### Sketch 1: UART Loopback
```text
==========================================
       ESP32-S3 UART Loopback Test        
==========================================
TX Pin : GPIO 17
RX Pin : GPIO 18
Baud   : 9600 (8N1)
Status : Ready. Transmitting PING every 1s...
------------------------------------------
PING
PING
PING
```

### Sketch 2: $\text{I}^2\text{C}$ Scanner
```text
==========================================
        ESP32-S3 I2C Bus Scanner          
==========================================
SDA Pin : GPIO 8
SCL Pin : GPIO 9
Scanning range: 0x01 to 0x7E...
------------------------------------------
Found device at 0x3C
------------------------------------------
Scan complete. Devices found: 1
==========================================
```

### Sketch 3: SPI Loopback
```text
==========================================
        ESP32-S3 SPI Loopback Test        
==========================================
SCK  : GPIO 12
MOSI : GPIO 11
MISO : GPIO 13
CS   : GPIO 10
Speed: 1 MHz | MSBFIRST | SPI Mode 0
Wiring: Connect GPIO 11 (MOSI) to GPIO 13 (MISO)
------------------------------------------
Sent 0xA5, received 0xA5
Sent 0xA5, received 0xA5
Sent 0xA5, received 0xA5
```

---

## 7. Laboratory Questions & Answers

### 1. Why does UART need both sides to use the same baud rate, while SPI uses a clock line?
> **Answer:** UART is an asynchronous protocol without a shared clock line, meaning both transmitter and receiver must agree on the exact bit duration (baud rate) beforehand to sample bits at the correct intervals. In contrast, SPI is synchronous and carries a dedicated clock line (`SCK`), allowing data bits to be sampled precisely on rising or falling clock edges regardless of preset timing.

### 2. Why can two devices with the same address not share one $\text{I}^2\text{C}$ bus?
> **Answer:** $\text{I}^2\text{C}$ relies on unique 7-bit device addresses broadcast over a shared serial data line (`SDA`). If two devices share the same address, both will acknowledge and attempt to pull the open-drain `SDA` line simultaneously, creating electrical bus contention and corrupted data.

### 3. Why is CS set HIGH between SPI transfers?
> **Answer:** The Chip Select (`CS`/`SS`) line is active-LOW, so driving it HIGH deselects the slave peripheral, resets its internal state machine, and forces its `MISO` output into a high-impedance (tri-state) condition so other devices can access the shared SPI bus cleanly.
