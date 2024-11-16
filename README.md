# Mikiot FW - (The Command-Line RTOS project for MCU)

## Table of Contents
- [Mikiot FW - (The Command-Line RTOS project for MCU)](#mikiot-fw---the-command-line-rtos-project-for-mcu)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Prerequisites](#prerequisites)
  - [Compilation](#compilation)
- [Getting Started with Serial Connection](#getting-started-with-serial-connection)
  - [Connection Setup](#connection-setup)
  - [Serial Settings](#serial-settings)
  - [Testing the Connection](#testing-the-connection)
  - [Features](#features)
    - [Blob Player](#blob-player)
    - [CRON Scheduler](#cron-scheduler)
    - [Command-Line Interface](#command-line-interface)
    - [Hardware Interaction](#hardware-interaction)
    - [Event-Driven GPRS Library](#event-driven-gprs-library)
  - [Contributing](#contributing)
  - [License](#license)

---

## Introduction

This is a fun little project for STM32 microcontrollers that lets you play around and experiment. We built it with an STM32F411 board, but it should work on other STM32F4xx boards too with some small tweaks.  

The project is a playground for learning and exploring embedded systems with features like real-time task management, peripheral control, and GPRS communication.

---

## Prerequisites

To get started, you’ll need the following:  

- **STM32CubeIDE**: The IDE for development and flashing.  
- **USB to UART converter**: For serial communication with the command-line interface.  
- **STM32F411 board** *(or another STM32F4xx board with small or no adjustments).*
- **App for serial communication** - You can choose your favorite. If you expect our suggestion it can be [HTerm](https://www.der-hammer.info/pages/terminal.html) it comes for Linux and Windows Operating Systems.

---

## Compilation

1. Open the project in **STM32CubeIDE**.  
2. Compile the project using the built-in compiler.  
3. Flash the firmware to the STM32F411 board using the debugger.  

The compilation process is straightforward, and no additional setup is required beyond the IDE and your hardware.

---

# Getting Started with Serial Connection

Once you've got the firmware loaded onto your board, let's set up communication! You'll need a USB-to-UART converter to talk to the board.

## Connection Setup
1. Connect your USB-to-UART converter to the board:
   - Converter's RX → Board's TX (GPIOB6)
   - Converter's TX → Board's RX (GPIOB7)

## Serial Settings
1. Open your favorite serial terminal program
2. Set it up with:
   - Baud Rate: 115200
   - Data bits: 8
   - Parity: None
   - Stop bits: 1

## Testing the Connection
1. Connect to the board
2. Type `help` and press Enter
3. If you see a list of available commands - congratulations, you're connected! 🎉

> 💡 For a complete list of pins and peripherals used in this project, check out the [Hardware Usage file](hw_usage.md).

Need help? Feel free to open an issue if you run into any problems!

---

## Features

### Blob Player  

The blob player is a lightweight interpreter idea for running small programs directly on the microcontroller. It allows you to execute commands and scripts through the CLI. Although still in initial phase, it’s functional enough to demonstrate the idea. 

**Current Limitations:**  
- Limited command set (extending is planned).  
- Simplistic syntax and error handling.

The blob player is designed to be extendable, so feel free to contribute or experiment with it further.

---

### CRON Scheduler  

A custom CRON-like scheduler lets you manage periodic tasks in the system. Unlike traditional CRON implementations, this version uses some non-standard approaches to task scheduling, making it both a learning tool and a fun feature of the project.  

**Key Features:**  
- Define and schedule periodic tasks.  
- Lightweight and embedded-friendly.  

---

### Command-Line Interface  

The command-line interface (CLI) is the main way to interact with the system in real time via UART. Using the CLI, you can:  
- Run blob player programs.  
- Manage tasks with the CRON scheduler.  
- Control hardware peripherals like ADC and GPIO.  
- Access a help menu with `help` to explore available commands.  

---

### Hardware Interaction  

This project provides tools to interact with STM32 peripherals through the CLI:  
- **ADC Control**: Read analog inputs and monitor data.  
- **GPIO Control**: Configure pins and toggle outputs directly.  

These features allow quick testing and prototyping without needing additional development.

---

### Event-Driven GPRS Library  

The project includes a custom event-driven GPRS library for communication. This feature can be used for IoT projects or remote data logging. The library supports sending and receiving messages, managing connections, and handling events seamlessly.

---

## Contributing  

Contributions to this project are welcomed! 

---

## License  

This project is open-source and licensed under the [MIT License](LICENSE). Feel free to use, modify, and distribute it as long as the original license is included.

---
