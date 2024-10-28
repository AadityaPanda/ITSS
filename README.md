# IoT Toll System Supervisor

This project uses **ESP32** and **Arduino Uno** to automate toll collection by detecting and validating vehicles with RFID tags. RFID readers identify registered vehicles, control a servo motor-operated gate, and record data in **Google Sheets** for real-time monitoring.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Components Required](#components-required)
- [Wiring and Hardware Setup](#wiring-and-hardware-setup)
- [How It Works](#how-it-works)
- [Installation and Setup](#installation-and-setup)
- [Code Overview](#code-overview)
  * [Arduino Uo Code](#arduino-uno-code)
  * [ESP32 Code](#esp32-code)
- [Photos and Videos](#photos-and-videos)
  * [Project Setup](#project-setup)
  * [Mobile App Setup](#mobile-app-setup)
  * [Demo Video](#demo-video)
- [Future Enhancements](#future-enhancements)

## Overview
This IoT-based toll collection system is designed to simplify toll management by automating vehicle recognition and payment deduction. With RFID technology, the system identifies registered vehicles, opens the toll gate, deducts the toll amount, and logs transaction data in **Google Sheets** via **ESP32**.

## Features
- **Automated Vehicle Detection:** RFID reader authenticates vehicle RFID tags.
- **Automated Gate Control:** Servo motor controls gate opening and closing.
- **Data Logging:** ESP32 logs all transactions in Google Sheets.
- **Security Checks:** Ensures that only authorized RFID tags are allowed.
- **Customizable Buzzer Alerts:** Alerts for valid and invalid entries.
  
## Components Required
- ESP32
- Arduino Uno
- RFID Module (MFRC522)
- Servo Motor
- Buzzer
- Liquid Crystal Display (LCD) with I2C
- Wires and Breadboard
- Power Supply

## Wiring and Hardware Setup
1. **ESP32 to Arduino Uno Communication**: Connect ESP32's RX/TX pins to Arduino's TX/RX.
2. **RFID Module to Arduino Uno**: Use SPI interface pins for communication.
3. **Servo Motor and Buzzer**: Connect to Arduino Uno's digital pins.
4. **LCD Screen**: Connect LCD to Arduino Uno through I2C.

**Note:** Refer to the wiring diagram below for exact pin connections.

![Circuit Diagram](https://github.com/user-attachments/assets/3eb5dfe1-39d4-4e7b-bc8f-289559f5a825)

## How It Works
1. **Card Detection**: When a vehicle’s RFID card is scanned, the system checks if it matches any registered card.
2. **Authentication**: If the card matches, the system displays the vehicle ID on the LCD, opens the gate, and sends data to ESP32.
3. **Data Logging**: ESP32 logs the transaction in Google Sheets, including date, time, and vehicle UID.
4. **Transaction Logging**: Each transaction is recorded in a Google Sheet, maintaining a record of all toll activity.

![Flowchart](https://github.com/user-attachments/assets/175c2886-e0e8-4a3f-afea-2d738290a0d8)

## Installation and Setup

### 1. Code and Upload
1. Upload **Arduino Uno Code** to the Arduino Uno:
   - Make sure to install `Wire.h`, `LiquidCrystal_I2C.h`, `Servo.h`, `MFRC522.h`, `SPI.h`, and `SoftwareSerial.h` libraries.
2. Upload **ESP32 Code** to the ESP32:
   - Ensure `WiFi.h`, `HTTPClient.h`, and `TimeLib.h` libraries are installed.

### 2. Configure Google Sheets for Data Logging
1. Create a **Google Sheet** and deploy a **Google Apps Script** as a web app. Set permissions to allow anyone to log data.
2. Replace the `serverName` URL in `ESP32` code with your Google Apps Script web app URL.

### 3. Testing
1. Connect all components and power on the system.
2. Test by scanning an authorized RFID tag; observe the gate operation, LCD message, and data logging in Google Sheets.

## Code Overview

### Arduino Uno Code
This code handles:
- **RFID Scanning**: Reads and validates RFID cards.
- **Gate Control**: Operates the gate servo motor for authorized vehicles.
- **LCD Display**: Displays vehicle information and status messages.
- **Buzzer Alerts**: Provides audible feedback based on vehicle status.

### ESP32 Code
This code handles:
- **WiFi Connection**: Connects to WiFi for Google Sheets communication.
- **RFID UID Validation**: Receives UID from Arduino, validates length.
- **Data Logging**: Sends JSON data to Google Sheets API for record-keeping.

## Photos and Videos
### Project Setup
![Hardware Setup](https://github.com/user-attachments/assets/7b945ba4-c151-4bfd-968c-ad3c9c1216a3)

### Mobile App Setup
![Mobile App](https://github.com/user-attachments/assets/e2d3197d-90f5-4a4d-a544-b3b0ea7d78de)

### Demo Video
https://github.com/user-attachments/assets/a4f83c5d-da9a-4461-9b8c-217969dc770b

## Future Enhancements
- **Multiple Payment Options**: Integrate with online payment gateways.
- **Vehicle Data Analytics**: Provide insights on toll collection data.
