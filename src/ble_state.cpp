#include <BLEDevice.h>
#include "ble_state.h"

// Define shared state EXACTLY ONCE here
bool deviceConnected = false;
volatile bool messageReceived = false;

void ServerCallbacks::onConnect(BLEServer* /*pServer*/) {
  deviceConnected = true;
  Serial.println("CONNECTED");
}

void ServerCallbacks::onDisconnect(BLEServer* /*pServer*/) {
  deviceConnected = false;
  Serial.println("DISCONNECTED");

  // Restart advertising so iOS can reconnect
  BLEDevice::startAdvertising();
  Serial.println("ADVERTISING RESTARTED");
}

void SignalCallbacks::onWrite(BLECharacteristic* pCharacteristic) {
  messageReceived = true;

  std::string value = pCharacteristic->getValue();

  Serial.print("MESSAGE RECEIVED, bytes=");
  Serial.println(value.length());

  // iOS is sending ASCII bytes (String -> .ascii)
  // Print it as text exactly as received.
  Serial.print("TEXT: ");
  Serial.println(value.c_str());
}