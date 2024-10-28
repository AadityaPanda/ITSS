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
- [Photos and Videos](#photos-and-videos)
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
  
(![3-Figure4-1](https://github.com/user-attachments/assets/138a8d2a-edbf-4aa4-9d84-67e183334222))

## How It Works
1. **Card Detection**: When a vehicle’s RFID card is scanned, the system checks if it matches any registered card.
2. **Authentication**: If the card matches, the system displays the vehicle ID on the LCD, opens the gate, and sends data to ESP32.
3. **Data Logging**: ESP32 logs the transaction in Google Sheets, including date, time, and vehicle UID.
4. **Transaction Logging**: Each transaction is recorded in a Google Sheet, maintaining a record of all toll activity.

## Installation and Setup

### 1. Configure Google Sheets for Data Logging
1. Create a **Google Sheet** and deploy a **Google Apps Script** as a web app. Set permissions to allow anyone to log data.
2. Replace the `serverName` URL in `ESP32` code with your Google Apps Script web app URL.

### 2. Code and Upload
1. Upload **ESP32 Code** to the ESP32:
   - Ensure `WiFi.h`, `HTTPClient.h`, and `TimeLib.h` libraries are installed.
2. Upload **Arduino Uno Code** to the Arduino Uno:
   - Make sure to install `Wire.h`, `LiquidCrystal_I2C.h`, `Servo.h`, `MFRC522.h`, `SPI.h`, and `SoftwareSerial.h` libraries.

### 3. Testing
1. Connect all components and power on the system.
2. Test by scanning an authorized RFID tag; observe the gate operation, LCD message, and data logging in Google Sheets.

## Code Overview

### ESP32 Code
This code handles:
- **WiFi Connection**: Connects to WiFi for Google Sheets communication.
- **RFID UID Validation**: Receives UID from Arduino, validates length.
- **Data Logging**: Sends JSON data to Google Sheets API for record-keeping.

### Arduino Uno Code
This code handles:
- **RFID Scanning**: Reads and validates RFID cards.
- **Gate Control**: Operates the gate servo motor for authorized vehicles.
- **LCD Display**: Displays vehicle information and status messages.
- **Buzzer Alerts**: Provides audible feedback based on vehicle status.

## Photos and Videos
- **Photos**: Add images showing the hardware setup and various system states.
- **Video**: A working demo video of the project in action.

### Project Setup Photo
(![WhatsApp Image 2024-07-17 at 20 58 59_c2296f88](https://github.com/user-attachments/assets/520e1b90-76ff-443d-900d-9045d79e2e8d)) 

### Mobile App Setup Photo
(![IMG_20240911_223336](https://github.com/user-attachments/assets/89b8bac7-f2ae-48f5-b0dd-ff5d91b62273)) 

### Demo Video
(https://github.com/user-attachments/assets/4a47b2a0-12c2-4a0b-9408-29a8dfe11535) 

## Future Enhancements
- **Multiple Payment Options**: Integrate with online payment gateways.
- **Vehicle Data Analytics**: Provide insights on toll collection data.
