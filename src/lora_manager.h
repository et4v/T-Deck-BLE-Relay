#ifndef LORA_MANAGER_H
#define LORA_MANAGER_H

#include <Arduino.h>
#include <RadioLib.h>
#include "utilities.h"

// this class exists so main does not need to know anything about radio pins
// or radiolib setup details
// main should only care that begin works and send sends something
class LoRaManager {
public:
  // bring up the lora radio and return whether it actually worked
  // this hides all the ugly radiolib config attempts from the rest of the program
  bool begin();

  // send exactly one byte over lora
  // this keeps packets tiny and predictable
  bool send(uint8_t value);

private:
  // Module describes how the sx1262 is wired to the esp32
  // these pins come from utilities so the whole project agrees on hardware
  Module module = Module(
    RADIO_CS_PIN,    // chip select pin for spi
    RADIO_DIO1_PIN,  // dio1 used for irq signaling
    RADIO_RST_PIN,   // reset pin for the radio
    RADIO_BUSY_PIN   // busy pin tells us when the radio is occupied
  );

  // sx1262 radio instance built on top of the module wiring
  // this object is what radiolib actually talks to
  SX1262 radio = SX1262(&module);
};

#endif