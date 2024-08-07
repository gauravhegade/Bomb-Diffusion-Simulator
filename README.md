# Bomb Diffusion Simulator

## Overview

The **Bomb Diffusion Simulator** is an interactive project that simulates the process of defusing a bomb using a set of electronic components and a microcontroller, specifically the LPC2148. The simulator integrates a proximity sensor, LCD display, matrix keyboard, and seven-segment display to create a realistic and engaging experience.

## Hardware Components

- **LPC2148 Microcontroller**: The central control unit of the system, responsible for handling logic operations, password entry, countdown timer, and interaction with other components.
- **Proximity Sensor**: Detects if a person is near the bomb, allowing the system to trigger the defusing process.
- **LCD Display**: Provides visual feedback to the user with messages such as "Enter pass:", "Bomb Diffused", or "Bomb Exploded".
- **4x4 Matrix Keyboard**: Used for entering the numerical password needed to defuse the bomb.
- **Seven-Segment Display**: Displays the countdown timer, showing the remaining time available to defuse the bomb.

## How It Works

1. **Initialization**:
   - When powered on or reset, the LPC2148 initializes all components, setting up the display and countdown timer.

2. **Display Setup**:
   - The LCD display shows the initial message: "Enter pass:".

3. **Countdown Timer & Proximity Sensor**:
   - The seven-segment display starts a countdown from 10 seconds, managed using interrupts. Simultaneously, the proximity sensor continuously monitors for a person approaching the device.

4. **Stopping the Countdown**:
   - If the proximity sensor detects a person, the countdown timer is halted.

5. **Password Entry**:
   - The user is prompted to enter a password using the 4x4 matrix keyboard. The entered password is compared to a predefined correct password.

6. **Outcome Display**:
   - Based on the entered password and the detection by the proximity sensor, the LCD display shows:
     - "Bomb diffused!" if the correct password is entered and the sensor detects a person.
     - "Bomb exploded!" if the incorrect password is entered despite the proximity sensor detecting a person.

## Getting Started

1. **Connect Hardware**:
   - Ensure all components are properly connected to the LPC2148 microcontroller as per the wiring diagram.

2. **Power On**:
   - Power on the system. The LCD display should initialize with "Enter pass:" and the countdown timer should start.

3. **Diffuse the Bomb**:
   - Approach the bomb to trigger the proximity sensor. Enter the correct password on the matrix keyboard within the countdown period to defuse the bomb.

4. **Outcome**:
   - Observe the message on the LCD display to determine if the bomb was successfully diffused or if it exploded.

## Acknowledgements

Special thanks to

- [@AnkithShetty397](https://github.com/AnkithShetty397)
- [@imonBanerjee1](https://github.com/imonBanerjee1)
- [@kir4nn](https://github.com/kir4nn)

that made this project possible; and

- [Dr. Badari Nath K](https://www.youtube.com/@drkbadarinath4636)

for providing us with this opportunity and his excellent guidance and support.

---

Happy defusing! 🚀
