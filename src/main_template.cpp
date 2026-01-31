//This main is just a test for the LoRa sending signals temp and will be changed

#include <Arduino.h>
#include "utilities.h"
#include "lora_manager.h"

LoRaManager lora;
uint8_t counter = 0;

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println();
  Serial.println("==== LORA ONLY TEST ====");

  pinMode(BOARD_POWERON, OUTPUT);
  digitalWrite(BOARD_POWERON, HIGH);
  delay(50);

  Serial.println("Power enabled");

  if (!lora.begin()) {
    Serial.println("LoRa init FAILED");
    while (true) delay(1000);
  }

  Serial.println("LoRa init OK");
}

void loop() {
  Serial.print("Sending ");
  Serial.println(counter);

  if (lora.send(counter)) Serial.println("TX ok");
  else Serial.println("TX fail");

  counter++;
  delay(1000);
}