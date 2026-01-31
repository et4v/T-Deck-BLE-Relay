#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>

#include "lora_manager.h"
#include "utilities.h"

// helper to print exactly what happened during each init attempt
// this exists so we can tell whether the radio is missing misconfigured or just mad
static void printAttempt(const char* label, int16_t state) {
  Serial.print(label);
  Serial.print(" -> ");
  Serial.println(state);

  // 0 means everything is good and the radio accepted the configuration
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("LoRa init OK");

  // this means SPI could not find the chip at all
  // usually wrong cs pin wrong spi pins or no power
  } else if (state == RADIOLIB_ERR_CHIP_NOT_FOUND) {
    Serial.println("chip not found");

  // this means the chip responded but rejected LoRa mode
  // usually tcxo regulator or modem configuration issue
  } else if (state == RADIOLIB_ERR_WRONG_MODEM) {
    Serial.println("wrong modem");

  // anything else means the radio did not like one of the parameters
  } else {
    Serial.println("init failed");
  }
}

bool LoRaManager::begin() {
  Serial.println();
  Serial.println("==== LORA ONLY TEST ====");

  // turn on the T Deck peripheral power rail
  // without this the LoRa chip is effectively dead
  pinMode(BOARD_POWERON, OUTPUT);
  digitalWrite(BOARD_POWERON, HIGH);
  delay(50);

  Serial.println("Power enabled");

  // print the pins so we know exactly what hardware we think we are talking to
  Serial.print("pins cs=");
  Serial.print(RADIO_CS_PIN);
  Serial.print(" dio1=");
  Serial.print(RADIO_DIO1_PIN);
  Serial.print(" rst=");
  Serial.print(RADIO_RST_PIN);
  Serial.print(" busy=");
  Serial.println(RADIO_BUSY_PIN);

  // explicitly start SPI on the T Deck pins
  // this is required because the LoRa radio is not on the ESP32 default SPI bus
  SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI, RADIO_CS_PIN);

  // make sure chip select is high when idle
  pinMode(RADIO_CS_PIN, OUTPUT);
  digitalWrite(RADIO_CS_PIN, HIGH);

  // radio parameters pulled from utilities so everyone uses the same values
  const float freq = 915.0;          // US band for this hardware
  const float bw   = RADIO_BANDWIDTH;
  const uint8_t sf = RADIO_SF;
  const uint8_t cr = RADIO_CR;
  const uint8_t sw = 0x12;           // standard LoRa sync word
  const int8_t pwr = RADIO_TX_POWER;
  const uint16_t pre = 8;

  // first attempt assumes the board has a tcxo running at 1.8 volts
  // this is common on sx1262 based boards
  // useRegulatorLDO false means dcdc mode
  int16_t s1 = radio.begin(freq, bw, sf, cr, sw, pwr, pre, 1.8, false);
  printAttempt("tcxo 1.8 dcdc", s1);

  if (s1 == RADIOLIB_ERR_NONE) {
    // dio2 controls the rf switch on many sx1262 boards
    radio.setDio2AsRfSwitch(true);
    radio.setCRC(true);
    return true;
  }

  // second attempt still uses tcxo at 1.8 volts
  // but switches the internal regulator to ldo mode
  // some boards require this instead of dcdc
  int16_t s2 = radio.begin(freq, bw, sf, cr, sw, pwr, pre, 1.8, true);
  printAttempt("tcxo 1.8 ldo ", s2);

  if (s2 == RADIOLIB_ERR_NONE) {
    radio.setDio2AsRfSwitch(true);
    radio.setCRC(true);
    return true;
  }

  // if we get here neither configuration worked
  // at this point the radio is responding but rejecting setup
  Serial.println("LoRa init FAILED");
  return false;
}

bool LoRaManager::send(uint8_t value) {
  // transmit exactly one byte
  // this is intentional to keep packets tiny and fast
  int16_t state = radio.transmit(&value, 1);

  Serial.print("tx ");
  Serial.print(value);
  Serial.print(" -> ");
  Serial.println(state);

  return state == RADIOLIB_ERR_NONE;
}