# T-Deck BLE Relay

PlatformIO project for the **LilyGO T-Deck (ESP32-S3)**.

This project acts as a **BLE relay**:
- iOS app → T-Deck via Bluetooth Low Energy
- T-Deck → future LoRa broadcast (planned)

---

## Project Goals

- Accept messages from an iOS app over BLE (write without response)
- Print connection state and received data over Serial
- Provide a clean base for forwarding messages over LoRa
- Be easy for a team to clone, build, and extend

---

## Hardware

- LilyGO T-Deck (ESP32-S3)
- USB connection for flashing + Serial monitor

---

## Software Stack

- **PlatformIO**
- **Arduino framework (ESP32)**
- BLE via Arduino-ESP32 core
- Custom board definition (included in repo)

---

## Repository Structure
