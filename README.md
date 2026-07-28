# microcontroller-sonar-radar.
Ultrasonic Sonar/Radar tracking system using a microcontroller, servo motor, and real-time Pygame GUI visualization.
# 📡 Microcontroller-Based Ultrasonic Radar System

A real-time ultrasonic sonar/radar tracking system built with a microcontroller, a servo motor, an HC-SR04 ultrasonic sensor, and a custom interactive graphical interface built in Python using Pygame.
###  System Preview

![Radar Interface](Screenshot%202026-07-09%20224936.png)

![Hardware Output](Screenshot%202026-07-09%20150526.png)
---

## 🌟 Key Features
* **180° Sweep Scan:** Smooth servo motor control sweeping continuously from 0° to 180°.
* **Real-time Target Tracking:** Ultrasonic distance detection mapped dynamically in degrees and centimeters.
* **Custom Pygame GUI:** Visual radar sweep animation showing detected obstacles with dynamic threat indicators and distance grid rings.
* **Serial Communication:** Fast and reliable UART data transmission between the microcontroller and Python host.

---

## 🛠️ Hardware & Tools Used
* **Microcontroller:** Microcontroller Board (esp8266)
* **Distance Sensor:** HC-SR04 Ultrasonic Sensor
* **Actuator:** SG90 / MG996R Servo Motor
* **Software:** C/C++ (Embedded), Python 3.x, Pygame, PySerial

---

## 📐 System Architecture

1. **Firmware:** Calculates obstacle distance from HC-SR04 echo duration ($d = \frac{v \cdot t}{2}$), steps the servo motor, and transmits formatted string data over UART (`angle,distance;`).
2. **GUI Host:** Listens on the serial port, parses angle/distance pairs in real time, and renders a live radar display using Pygame.

---

## 🚀 Getting Started

### Prerequisites
Install the required Python packages:
```bash
pip install pygame pyserial
