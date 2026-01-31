#include <Arduino.h>
#include <BLEDevice.h>
#include "ble_state.h"

// these are shared flags used by main
// they live here so they are defined exactly once
// the header only declares them as extern

bool deviceConnected = false;
volatile bool messageReceived = false;

// called automatically when a phone connects over BLE
void ServerCallbacks::onConnect(BLEServer* /*pServer*/) {
  deviceConnected = true;
  Serial.println("BLE connected");
}

// called automatically when the phone disconnects
void ServerCallbacks::onDisconnect(BLEServer* /*pServer*/) {
  deviceConnected = false;
  Serial.println("BLE disconnected");
}

// called whenever the phone writes to the signal characteristic
void SignalCallbacks::onWrite(BLECharacteristic* /*pCharacteristic*/) {
  // we do not care about payload content yet
  // this flag is just a signal to main that something arrived
  messageReceived = true;
  Serial.println("BLE message received");
}