Automated Vehicle Temperature Controlling and Security System

Overview

This project is an advanced IoT-based system designed to monitor and control the temperature inside vehicles and ensure their security. It utilizes various sensors, a microcontroller, and a web application to provide real-time data access and control. The system operates autonomously, ensuring optimal vehicle environment and security without requiring external power sources.

Features
Temperature Control System
    NodeMCU Board: Central processing unit for data and component control.
    DHT11 Sensor: Measures the temperature inside the vehicle.
    12V Fan: Cools the vehicle when the temperature exceeds 30°C.
    Motor Controller: Manages fan operations based on temperature data.
    Rechargeable Battery: Powers the system independently of the vehicle's battery.

Security System
    Motion Sensor: Detects unauthorized movement in or around the vehicle.
    Email Alerts: Sends notifications to the user when motion is detected.

Future Enhancements:
    Solar Power Integration: Provides a sustainable power source.
    Image Processing: Captures and analyzes images upon motion detection.

GPS Tracking
    Neo 6M GPS Module: Provides real-time location data of the vehicle.
    Web Application Access: Allows users to monitor vehicle location in real-time.

Web Application
    Frontend: Built with React.js using the MERN stack.
    Backend: Powered by Node.js and Express.js.
    Communication Protocols: Utilizes WebSocket and HTTP for data transmission.
    User Authentication: Secure login system for accessing vehicle data.
    Real-Time Data Display: Monitors temperature and motion data live.

Technologies Used
    Microcontroller: NodeMCU
    Sensors: DHT11 (Temperature), Neo 6M (GPS), Motion Sensor
    Components: 12V Fan, Motor Controller, Rechargeable Battery
    Backend Development: Node.js, Express.js
    Frontend Development: React.js (MERN Stack)
    Communication: WebSocket, HTTP
    IoT Integration: Hardware and software integration for efficient performance.

Installation and Setup
Prerequisites
    Node.js
    npm (Node Package Manager)
    MongoDB (for database)
