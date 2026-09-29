# Wi-Fi Access Control System

An ESP32-based Wi-Fi Access Control System developed using Arduino IDE.

## Project Description

This project provides a simple access control system using an ESP32. 
Users can request access through a web interface, and the system can 
grant or deny access based on authentication.

## Features

- Wi-Fi connectivity
- Web-based access control
- Username and password authentication
- Access granted/denied indication
- Door open/closed detection
- Access request button
- Exit request button
- Tamper detection
- Green and red LED indicators
- Buzzer alarm
- ESP32-based control

## Hardware Used

- ESP32 Dev Module
- Door/Magnetic Sensor
- Push Buttons
- Green LED
- Red LED
- Buzzer
- Resistors
- Breadboard and jumper wires

## Software Used

- Arduino IDE 2.3.10
- ESP32 Board Package
- Arduino WebServer Library

## Pin Connections

| Component | ESP32 GPIO |
|---|---:|
| Door Sensor | GPIO 21 |
| Access Button | GPIO 19 |
| Exit Button | GPIO 22 |
| Tamper Button | GPIO 23 |
| Green LED | GPIO 25 |
| Red LED | GPIO 26 |
| Buzzer | GPIO 27 |

## Authentication

The prototype uses a web-based login system.  
The correct credentials allow access, while incorrect credentials result in access denial and an alarm indication.

## Project Status

The prototype successfully demonstrates authentication, access control, door monitoring, and tamper detection.

## Future Improvements

- Persistent event history
- Database integration
- Multiple user accounts
- Improved security
- Mobile-friendly interface
