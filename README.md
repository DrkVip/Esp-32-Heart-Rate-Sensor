# ESP32 Heart Rate Monitor with MAX30105

## Overview
This project uses an ESP32 microcontroller and a MAX30105 pulse oximeter sensor to measure heart rate and display the data in real-time on a web page using an embedded web server.

## Features
- Reads heart rate data from MAX30105.
- Hosts a web server on ESP32.
- Displays real-time heart rate using Chart.js.
- Connects to WiFi for remote monitoring.

## Hardware Requirements
- ESP32
- MAX30105 Pulse Oximeter Sensor
- Jumper wires
- Breadboard (optional)

## Software Requirements
- Arduino IDE
- ESP32 board package
- MAX30105 sensor library
- ESPAsyncWebServer library
- AsyncTCP library

## Installation & Setup
1. Install the ESP32 board package in Arduino IDE.
2. Install required libraries from the Arduino Library Manager.
3. Clone this repository and open the `.ino` file in Arduino IDE.
4. Update `WIFI_SSID` and `WIFI_PASSWORD` in the code.
5. Compile and upload the code to the ESP32.
6. Open the Serial Monitor to find the ESP32's IP address.
7. Access the web interface via the IP address in a browser.

## Wiring Guide
| MAX30105 Pin | ESP32 Pin |
|-------------|-----------|
| VIN         | 3.3V      |
| GND         | GND       |
| SDA         | GPIO 21   |
| SCL         | GPIO 22   |

## Usage
- Once the ESP32 is connected to WiFi, visit its IP address in a browser.
- The web page will show a real-time graph of heart rate readings.

## Troubleshooting
- If the sensor is not detected, check the wiring.
- Ensure WiFi credentials are correct.
- Install all necessary libraries before compiling.

## License
This project is open-source under the MIT License.


https://github.com/DrkVip/Esp-32-Heart-Rate-Sensor/blob/main/Output.jpg?raw=true

