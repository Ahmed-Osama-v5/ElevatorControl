# Elevator Control System

A complete elevator control system implementation for ATmega32 microcontroller, featuring a layered software architecture with MCAL, HAL, and Application layers.

## 📋 Overview

This project implements a fully functional elevator controller designed for ATmega32 MCU. The system manages elevator operations including floor requests, motor control, door operations, and user interface through a structured, modular codebase.

## 🏗️ Project Structure

ElevatorControl/

├── App/ # Application layer - Main elevator logic

├── Common/ # Common definitions, types, and utilities

├── HAL/ # Hardware Abstraction Layer - Drivers for external components

├── MCAL/ # Microcontroller Abstraction Layer - MCU peripheral drivers

└── CMakeLists.txt

## 🔧 Hardware Requirements

- **Microcontroller**: ATmega32
- **Clock Frequency**: 8 MHz (internal rc)

## 🚀 Features

- Up to 16 floors
- Request queuing system
- Motor direction control (up/down)
- Floor position tracking
- Safety mechanisms
- Modular and scalable architecture
- Clean separation of concerns (MCAL/HAL/App layers)

## 📦 Software Architecture

### MCAL (Microcontroller Abstraction Layer)
Low-level drivers for ATmega32 peripherals:
- GPIO
- Timers
- Interrupts
- EEPROM driver

### HAL (Hardware Abstraction Layer)
High-level drivers for external hardware:
- Relay manager (Motor driver through the off-board classic control panel).
- Button driver
- LCD display (you can choose either normal lcd or i2c lcd)
- Sensor manager
- Segment driver

### Application Layer
Main elevator control logic:
- Elevator controller
- Call (floor request) handler
- LED controller
- Motion controller
- Menu module (to handle menu items in settings programming mode)

## 🛠️ Build Instructions

### Prerequisites
- AVR-GCC toolchain
- CMake (version 3.x or higher)
- AVR programmer (USBasp, AVRISP, etc.)

### Building the Project

```bash / cmd
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake -G "Ninja"

# Build
ninja
```
### Hardware mapping
Please check included schematics PDF file (not uploaded yet).

🧪 Testing

The system can be tested by:

- Simulating
- Hardware testing with actual ATmega32 board
- Unit testing individual modules

📄 License

This project is open source and available under the MIT License.

🤝 Contributing

Contributions, issues, and feature requests are welcome!
