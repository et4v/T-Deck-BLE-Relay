#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include "ble_state.h"

// Must match iOS app
#define SERVICE_UUID          "19B10000-E8F2-537E-4F6C-D104768A1214"
#define SIGNAL_CHARACTERISTIC "19B10001-E8F2-537E-4F6C-D104768A1214"

BLEServer* pServer = nullptr;
BLECharacteristic* pSignalCharacteristic = nullptr;

void setup() {
  Serial.begin(115200);
  Serial.setTxTimeoutMs(0);
  delay(1500);

  Serial.println("BLE minimal receive test");
  Serial.println("ADVERTISING...");

  BLEDevice::init("PitchCom-Relay");

  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  BLEService* pService = pServer->createService(SERVICE_UUID);

  // IMPORTANT: include WRITE_NR for iOS writeWithoutResponse
  pSignalCharacteristic = pService->createCharacteristic(
    SIGNAL_CHARACTERISTIC,
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_WRITE_NR
  );
  pSignalCharacteristic->setCallbacks(new SignalCallbacks());

  pService->start();

  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
}

void loop() {
  Serial.print("STATE=");
  Serial.print(deviceConnected ? "CONNECTED" : "WAITING");
  Serial.print(" | MSG=");
  Serial.println(messageReceived ? "YES" : "NO");
  delay(1000);
}