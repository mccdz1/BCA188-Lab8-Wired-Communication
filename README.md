# Laboratory Activity 8: Wired Communication

**Course:** BCA188 (AY26-27 JFV)  
**Task:** Verify the three wired interfaces on the ESP32 board: UART, $\text{I}^2\text{C}$, and SPI.  
**Microcontroller Target:** ESP32-S3-N16R8 (16MB Octal Flash, 8MB Octal PSRAM)  
**Prototyping Medium:** Solderless Breadboard  

---

## 1. Pin Table

All GPIOs were chosen to avoid internal Octal Flash/PSRAM lines (GPIO 33–37) and strapping pins (GPIO 0, 45, 46).

| Signal | GPIO Used | Function / Test Connection | Notes |
| :--- | :---: | :--- | :--- |
| **UART TX** | **GPIO 17** | Serial2 TX $\rightarrow$ Jumper to RX | Output to loopback line |
| **UART RX** | **GPIO 18** | Serial2 RX $\leftarrow$ Jumper from TX | Input from loopback line |
| **$\text{I}^2\text{C}$ SDA** | **GPIO 8** | $\text{I}^2\text{C}$ Serial Data $\rightarrow$ Module SDA | Open-drain data line |
| **$\text{I}^2\text{C}$ SCL** | **GPIO 9** | $\text{I}^2\text{C}$ Serial Clock $\rightarrow$ Module SCL | Clock line |
| **SPI SCK** | **GPIO 12** | SPI Serial Clock | Bus clock (1 MHz) |
| **SPI MOSI** | **GPIO 11** | Master Out Slave In $\rightarrow$ Jumper to MISO | Output to loopback line |
| **SPI MISO** | **GPIO 13** | Master In Slave Out $\leftarrow$ Jumper from MOSI | Input from loopback line |
| **SPI CS** | **GPIO 10** | SPI Chip Select (Active LOW) | Bus control line |

---

## 2. Wiring Diagram & Breadboard Setup

### Schematic Layout

```text
                  ESP32-S3-N16R8 Breadboard Wiring
               +--------------------------------------+
               |            [USB-C PORT]              |
               |                                      |
   UART Test:  | [GPIO 17] (TX) -----\ (Jumper Wire)  |
               | [GPIO 18] (RX) <----/                |
               |                                      |
   I2C Module: | [3V3] / [5V] --------> VCC (I2C LCD) |
               | [GND]     ----------> GND (I2C LCD)  |
               | [GPIO 8]  ----------> SDA (I2C LCD)  |
               | [GPIO 9]  ----------> SCL (I2C LCD)  |
               |                                      |
   SPI Test:   | [GPIO 10] ----------> CS (Active L)  |
               | [GPIO 12] ----------> SCK            |
               | [GPIO 11] (MOSI) ----\ (Jumper Wire) |
               | [GPIO 13] (MISO) <---/               |
               +--------------------------------------+
```

### Circuit Photo
*(Place your breadboard circuit photo in `assets/circuit_setup.jpg`)*

![Breadboard Circuit Setup](assets/circuit_setup.jpg)

---

## 3. Expected versus Observed Results Table

| Test | Expected Output | Observed Output | Status |
| :--- | :--- | :--- | :---: |
| **UART with jumper** | Prints `PING` once every second | Echoes `PING` every second | PASS |
| **UART without jumper** | No characters received / transmission silent | Output stops; no characters received | PASS |
| **$\text{I}^2\text{C}$ with module** | `Found device at 0x..` | `Found device at 0x27` (I2C LCD) | PASS |
| **$\text{I}^2\text{C}$ without module** | `No I2C devices found` | `No I2C devices found` | PASS |
| **SPI with jumper** | `Sent 0xA5, received 0xA5` | `Sent 0xA5, received 0xA5` | PASS |
| **SPI without jumper** | `Sent 0xA5, received 0x00` *(or `0xFF`)* | `Sent 0xA5, received 0x00` | PASS |
| **Sketch at 9600, monitor at 115200** | Garbled / corrupt glyphs (`?` / gibberish) | Corrupted characters / unreadable symbols | PASS |
| **Sketch at 9600, monitor at 9600** | Clean, readable text output | Clean, legible text output | PASS |

---

## 4. Serial Monitor Output Logs

### Sketch 1: UART Loopback (`uart_loopback/uart_loopback.ino`)
```text
==========================================
       ESP32-S3 UART Loopback Test        
==========================================
TX Pin : GPIO 17
RX Pin : GPIO 18
Baud   : 9600 (8N1)
Wiring : Connect GPIO 17 (TX) to GPIO 18 (RX)

PING
PING
PING
PING
PING
```
<img width="600" height="179" alt="image" src="https://github.com/user-attachments/assets/3b7ddb5f-f85e-49fd-86a4-2e106325006d" />

### Sketch 2: $\text{I}^2\text{C}$ Scanner (`i2c_scanner/i2c_scanner.ino`)

#### With Module:
```text
------------------------------------------
       ESP32-S3 I2C Bus Scanner          
------------------------------------------
SDA Pin : GPIO 8
SCL Pin : GPIO 9
Scanning range: 0x01 to 0x7E...

Found device at 0x27

Scan complete. Devices found: 1
==========================================
```
<img width="466" height="146" alt="image" src="https://github.com/user-attachments/assets/8b9b6ff0-b6eb-4a2a-9a00-cf47e7172a83" />

#### Without Module:
```text
------------------------------------------
       ESP32-S3 I2C Bus Scanner          
------------------------------------------
SDA Pin : GPIO 8
SCL Pin : GPIO 9
Scanning range: 0x01 to 0x7E...
------------------------------------------
No I2C devices found
==========================================
```

### Sketch 3: SPI Loopback (`spi_loopback/spi_loopback.ino`)

#### With Jumper (GPIO 11 to GPIO 13):
```text
------------------------------------------
        ESP32-S3 SPI Loopback Test        
------------------------------------------
SCK  : GPIO 12
MOSI : GPIO 11
MISO : GPIO 13
CS   : GPIO 10
Speed: 1 MHz | MSBFIRST | SPI Mode 0

Sent 0xA5, received 0xA5
Sent 0xA5, received 0xA5
Sent 0xA5, received 0xA5
Sent 0xA5, received 0xA5
```

#### Without Jumper:
```text
Sent 0xA5, received 0x00
Sent 0xA5, received 0x00
```

---

## 5. Answers to Laboratory Questions

### 1. Why does UART need both sides to use the same baud rate, while SPI uses a clock line?
> **Answer:** UART is an asynchronous protocol without a shared clock line, meaning both transmitter and receiver must agree on the exact bit duration (baud rate) beforehand to sample incoming bits at predictable intervals. In contrast, SPI is synchronous and carries a dedicated clock line (`SCK`), allowing data bits to be sampled precisely on rising or falling clock edges regardless of preset timing.

### 2. Why can two devices with the same address not share one $\text{I}^2\text{C}$ bus?
> **Answer:** $\text{I}^2\text{C}$ uses unique 7-bit device addresses broadcast across the shared open-drain serial data line (`SDA`). If two devices share the same address, both will acknowledge and attempt to drive the `SDA` line simultaneously, creating electrical bus contention and corrupted communications.

### 3. Why is CS set HIGH between SPI transfers?
> **Answer:** The Chip Select (`CS`/`SS`) line is active-LOW, so driving it HIGH deselects the slave peripheral, resets its internal SPI communication state machine, and places its `MISO` output line into a high-impedance (tri-state) condition so other devices can access the shared SPI bus cleanly.
