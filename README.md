# 12-bit Synthesizer

A dual-microcontroller synthesizer project using AVR microcontrollers to create a digital audio synthesizer with keyboard input.

## Overview

This project consists of two main components:
- **MEGA16-synthIO**: Handles 4x4 keyboard input and I2C communication
- **MEGA328-synth**: Generates 12-bit audio synthesis using sine wave lookup tables

## Architecture

### MEGA16 (Input/Output Controller)
- Reads 4x4 matrix keyboard input
- Displays current key state on LCD
- Transmits key data via custom serial protocol to MEGA328
- Handles interrupt-driven data transmission

### MEGA328 (Audio Synthesis)
- Receives note data via I2C communication
- Generates audio using pre-calculated sine wave lookup table
- Outputs 12-bit audio samples at 44.1kHz sampling rate
- Supports musical notes from C to C2 (523Hz - 1047Hz)

## Features

- **Real-time Audio**: 44.1kHz sampling rate with timer-based interrupts
- **Musical Scale**: Supports 8 musical notes (C, D, E, F, G, A, B, C2)
- **Hardware Interface**: 4x4 matrix keyboard with LCD display
- **Inter-MCU Communication**: Custom protocol for reliable data transfer
- **12-bit Audio Output**: High-quality audio generation

## Hardware Requirements

- 2x AVR microcontrollers (ATmega16 and ATmega328)
- 4x4 matrix keyboard
- LCD display (44780 compatible)
- I2C/TWI connection between microcontrollers
- Audio output circuitry (DAC)

## Technical Details

- **Audio Generation**: 360-point sine wave lookup table
- **Frequency Control**: Variable frequency division for different notes
- **Communication**: I2C protocol for inter-MCU data transfer
- **Timing**: Precise interrupt-driven sampling and communication
