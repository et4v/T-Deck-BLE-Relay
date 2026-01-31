#pragma once

#include <Arduino.h>
#include <BLEServer.h>
#include <BLECharacteristic.h>

// These exist, but are defined in ble_state.cpp (NOT here)
extern bool deviceConnected;
extern volatile bool messageReceived;

// Callback types
class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) override;
  void onDisconnect(BLEServer* pServer) override;
};

class SignalCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) override;
};