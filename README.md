# AI-Powered Industrial Safety Intelligence for Zero-Harm Operations

## Overview
This project is a smart industrial safety monitoring and alert system built using ESP32, a gas sensor, DHT22, a PIR sensor, RFID, a buzzer, relay and LEDs. It is designed to detect compound risk conditions in industrial environments and trigger alerts before accidents happen.

## Problem Statement
Heavy industries often have sensors and safety systems, but the data is not always connected to decision-making in real time. This project creates a unified safety intelligence prototype that combines sensor data, worker presence, and access validation to identify dangerous situations early.

## Features
- Gas leak detection
- Temperature and humidity monitoring
- Motion detection
- RFID-based worker authentication
- Risk-level indication using LEDs
- Buzzer-based alert system
- Relay trigger for emergency action
- Compound risk detection logic

## Components Used
- ESP32 DevKit V4
- Wokwi Gas Sensor
- DHT22 Sensor
- PIR Motion Sensor
- MFRC522 RFID Reader
- Buzzer
- Relay Module
- Red, Yellow, Green LEDs
- Resistors

## Pin Connections
- Gas Sensor AOUT -> GPIO14
- DHT22 DATA -> GPIO12
- PIR OUT -> GPIO13
- Buzzer -> GPIO15
- Red LED -> GPIO17
- Yellow LED -> GPIO16
- Green LED -> GPIO2
- Relay IN -> GPIO21
- RFID SDA -> GPIO5
- RFID SCK -> GPIO18
- RFID MOSI -> GPIO23
- RFID MISO -> GPIO19
- RFID RST -> GPIO22

## Working Principle
The system reads gas concentration, environmental conditions, motion, and RFID data. Based on these combined inputs, it determines whether the area is safe, warning-level, or dangerous. If high risk is detected, the buzzer and relay are activated and the red LED turns on.

## Files in This Repository
- `diagram.json` - Wokwi circuit design
- `sketch.ino` - Arduino code for ESP32
- `README.md` - Project documentation

## How to Run
1. Open the Wokwi simulation link.
2. Start the simulation.
3. Observe the sensor values and alerts.
4. Scan RFID tags in the simulation if configured.
5. Monitor output in the serial console.

## Future Enhancements
- Add cloud dashboard
- Add geospatial plant heatmap
- Add AI-based anomaly prediction
- Add permit-to-work intelligence
- Add compliance report generation

## Author
YAGATI SURYA VENKATA SAI KRISHNA
ECE (2024), Vel Tech University, Chennai
